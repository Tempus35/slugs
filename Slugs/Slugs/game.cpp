#include "game.h"
#include "player.h"
#include "aiplayer.h"

Game::Game()
{

	world = NULL;
	updateManager = new UpdateManager();
	fxManager = new FXManager();

	for (int i = 0; i < GameBool_LAST; ++ i)
		gameBools.push_back(GetGameDefaultBool((GameBool)i));

	for (int i = 0; i < GameFloat_LAST; ++ i)
		gameFloats.push_back(GetGameDefaultFloat((GameFloat)i));

	for (int i = 0; i < GameInt_LAST; ++ i)
		gameInts.push_back(GetGameDefaultInt((GameInt)i));

	state = GameState_None;

	camera = new Camera();

	noAction = true;

	// Load initial resources
	LoadResourcesForState(GameState_None);

}

Game::~Game()
{

	for (unsigned int i = 0; i < players.size(); ++ i)
		SafeDelete(players[i]);
	

	SafeDelete(camera);
	SafeDelete(world);
	SafeDelete(updateManager);
	SafeDelete(fxManager);

}

void Game::Update(float elapsedTime)
{

	if (!GetGameBool(GameBool_Pause))
	{

		// Update the active player object
		if (activePlayer != -1)
			players[activePlayer]->Update(elapsedTime);

		// Update the camera
		if (camera->Update(elapsedTime))
			world->CameraMoved(camera->GetPosition());

		// Update the update manager
		if (updateManager)
			updateManager->Update(elapsedTime);

		// Update world
		if (world)
			noAction = !world->Update(elapsedTime);

		if (fxManager)
			fxManager->Update(elapsedTime);

	}

}

void Game::Render()
{

	// Set out camera
	Renderer::Get()->SetCamera(camera);

	// Render the world
	if (world)
		world->Render();

	//
	// World space debugging overlay
	//

	#ifdef _DEBUG

		if (GetGameBool(GameBool_Debug))
		{

			//
			// Show debugging info about the object under the cursor
			//

			Object* object = world->SelectedObject();
			
			if (!object)
			{

				Vec2f worldPos = camera->GetWorldPosition(cursorPosition.x, cursorPosition.y);
				object = world->GetObjectAtPosition(worldPos);

			}

			if (object)
			{

				ObjectType objectType = object->GetType();
				const Vec2f& objectPos = object->GetPosition();
				const Vec2f& objectVel = object->GetVelocity();

				char debugString[1024];
				sprintf_s(debugString, 1024, "[%s]\n\nPosition: %.1f, %.1f\nVelocity: %.1f, %.1f (%.1f)\n\nAtRest = %s", ObjectTypeToString(objectType).c_str(), objectPos.x, objectPos.y, objectVel.x, objectVel.y, object->GetSpeed(), BoolToString(object->IsAtRest()).c_str());
				
				if (objectType == ObjectType_Projectile)
					sprintf_s(&debugString[strlen(debugString)], 1024 - strlen(debugString), "\n\nTimer: %.1f", ((Projectile*)object)->GetTimer());
				
				Renderer::Get()->DrawDebugHint(objectPos + Vec2f(20.0f, -20.0f), debugString, Color(255, 255, 255));

			}

		}

	#endif

	// Render world space debugging overlay
	Renderer::Get()->DebugDraw();

	//
	// Screen space debugging overlay
	//

	// Don't use the camera
	Renderer::Get()->SetCamera(NULL);

	Player* currentPlayer = GetCurrentPlayer();

	if ((currentPlayer) && (currentPlayer->GetCurrentSlug()))
	{

		char debugInfo[1024];
		sprintf_s(debugInfo, 1024, "%s - %s (%.1f)", currentPlayer->GetName().c_str(), currentPlayer->GetCurrentSlug()->GetName().c_str(), currentPlayer->GetTurnTimeRemaining());
		Renderer::Get()->RenderText(10, 40, NULL, debugInfo, 16.0f, Color(255, 255, 255));

	}

	float playerInfoY = 70.0f;
	char buffer[256];
	for (unsigned int i = 0; i < players.size(); ++ i)
	{

		// Player name
		Renderer::Get()->RenderText(10, playerInfoY, NULL, players[i]->GetName(), 16.0f, (players[i]->GetType() == PlayerType_Local ? Color::white : Color::red), FontFlag_Bold);
		playerInfoY += 20.0f;

		// Player's teams
		for (unsigned int j = 0; j < players[j]->GetNumTeams(); ++ j)
		{

			Team* team = players[i]->GetTeam(j);

			// Team name
			sprintf_s(buffer, 256, "%s (%i)", team->GetName().c_str(), team->GetTotalHitpoints());
			Renderer::Get()->RenderText(10, playerInfoY, NULL, buffer, 16.0f, team->GetColor(), FontFlag_Bold);
			playerInfoY += 20.0f;

		}

		playerInfoY += 10.0f;

	}

}

World* Game::GetWorld() const
{

	return world;

}

UpdateManager* Game::GetUpdateManager() const
{

	return updateManager;

}

FXManager* Game::GetFXManager() const
{

	return fxManager;

}

bool Game::KeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (state == GameState_Game)
	{

		Slug* selectedSlug;
		bool turnEnding = GetCurrentPlayer()->IsTurnEnding();

		// Only allow the user to control their slug on their own turn
		if ((Game::Get()->GetCurrentPlayer()) && (GetCurrentPlayer()->GetType() == PlayerType_Local))
			selectedSlug = Game::Get()->GetCurrentPlayer()->GetCurrentSlug();
		else
			selectedSlug = NULL;

		switch (key)
		{

		//
		// Debugging
		//

		#ifdef _DEBUG

		case sf::Key::L:

			// Toggle locking camera to the level
			ToggleGameBool(GameBool_LockCameraToLevel);

			break;

		case sf::Key::K:

			// Kill the current slug
			if (selectedSlug)
				selectedSlug->SetHitpoints(-1);
			
			break;

		case sf::Key::D:

			// Toggle debug overlay
			ToggleGameBool(GameBool_Debug);

			break;

		case sf::Key::E:

			// Force the current players turn to end
			Game::EndTurn();

			break;

		#endif

		//
		// Game Controls
		//

		case sf::Key::P:

			ToggleGameBool(GameBool_Pause);

			break;
			
		//
		// Team Controls
		//

		case sf::Key::Tab:

			// Cycles through available slugs if the feature is enabled
			if ((GetGameBool(GameBool_CanChooseSlug)) && (selectedSlug))
			{

				if ((GetGameBool(GameBool_CanUseMultipleSlugsPerTurn)) || (!GetCurrentPlayer()->HasActed()))
						GetCurrentPlayer()->SelectNextSlug();
				else
					GetCurrentPlayer()->MoveCameraToActiveSlug();

			}
			else
					GetCurrentPlayer()->MoveCameraToActiveSlug();

			break;
	
		//
		// Slug Controls
		//

		case sf::Key::Left:

			if (selectedSlug)
				selectedSlug->StartMovingLeft();

			break;

		case sf::Key::Right:

			if (selectedSlug)
				selectedSlug->StartMovingRight();

			break;

		case sf::Key::Up:

			if (selectedSlug)
				selectedSlug->StartAimingUp();

			break;

		case sf::Key::Down:
			
			if (selectedSlug)
				selectedSlug->StartAimingDown();

			break;

		case sf::Key::Return:

			if (selectedSlug)
				selectedSlug->Jump();

			break;

		case sf::Key::Space:

			if ((selectedSlug) && (!turnEnding))
				selectedSlug->StartCharging();

			break;

		case sf::Key::Num1:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Bazooka);

			break;

		case sf::Key::Num2:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Grenade);

			break;

		case sf::Key::Num3:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Shotgun);

			break;

		case sf::Key::Num4:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Machinegun);

			break;

		case sf::Key::Num5:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Mine);

			break;

		case sf::Key::Num6:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_HomingMissile);

			break;

		case sf::Key::Num9:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Airstrike);

			break;

		case sf::Key::Num0:

			if (selectedSlug)
				selectedSlug->ArmSelf(WeaponType_Teleporter);

			break;

		}

	}

	return true;

}

bool Game::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (state == GameState_Game)
	{
		
		Slug* selectedSlug;
		bool turnEnding = GetCurrentPlayer()->IsTurnEnding();

		// Only allow the user to control their slug on their own turn
		if ((Game::Get()->GetCurrentPlayer()) && (GetCurrentPlayer()->GetType() == PlayerType_Local))
			selectedSlug = Game::Get()->GetCurrentPlayer()->GetCurrentSlug();
		else
			selectedSlug = NULL;

		switch (key)
		{

			case sf::Key::Left:
			case sf::Key::Right:

				if (selectedSlug)
					selectedSlug->StopMoving();
			
				break;

			case sf::Key::Up:
			case sf::Key::Down:

				if (selectedSlug)
					selectedSlug->StopAiming();

				break;

			case sf::Key::Space:

				if ((selectedSlug) && (!turnEnding))
					selectedSlug->Fire();

				break;

		}

	}

	return true;

}

bool Game::MouseDown(const Vec2i& position, sf::Mouse::Button button)
{

	if (state == GameState_Game)
	{

		if (button == sf::Mouse::Left)
		{

			Slug* selectedSlug;

			if ((Game::Get()->GetCurrentPlayer()) && (GetCurrentPlayer()->GetType() == PlayerType_Local))
				selectedSlug = Game::Get()->GetCurrentPlayer()->GetCurrentSlug();
			else
				selectedSlug = NULL;

			if (selectedSlug)
				selectedSlug->SetTarget(camera->GetWorldPosition(position.x, position.y));

		}
			

		#ifdef _DEBUG

		if (GetGameBool(GameBool_Debug))
		{

			if (button == sf::Mouse::Left)
			{

				Vec2f pickPosition = camera->GetWorldPosition(position.x, position.y);
				Object* pickedObject = Game::Get()->GetWorld()->GetObjectAtPosition(pickPosition);

				if (pickedObject)
					world->SelectObject(pickedObject);
				else
					world->SelectObject(NULL);

			}
			else if (button == sf::Mouse::Middle)
			{

				Object* selectedObject = world->SelectedObject();

				if (selectedObject)
				{

					Vec2f pickPosition = camera->GetWorldPosition(position.x, position.y);
					world->SelectedObject()->SetPosition(pickPosition);

					if (selectedObject->GetType() == ObjectType_Projectile)
					{

						Projectile* projectile = (Projectile*)selectedObject;

						if (projectile->GetTimer() >= 0.0f)
							projectile->SetTimer(5.0f);

					}

				}

			}

		}

		#endif

	}

	return true;

}

bool Game::MouseUp(const Vec2i& position, sf::Mouse::Button button)
{

	return true;

}

bool Game::MouseMoved(bool left, bool right, bool middle, const Vec2i& from, const Vec2i& to)
{

	// Store the new cursor position
	cursorPosition = to;

	if (state == GameState_Game)
	{

		if (right)
		{

			//
			// RMB moves the camera
			//

			Vec2i delta = from - to;
			camera->Move(delta.x, -delta.y);

			// Lets the world know the camera moved
			world->CameraMoved(camera->GetPosition());

		}

	}

	return true;

}

void Game::LoadResourcesForState(GameState gameState)
{

	ResourceManager* resourceManager = ResourceManager::Get();

	if (gameState == GameState_None)
	{

		//
		// These resources are loaded at app start
		//

		// Fonts
		resourceManager->AddResource("font_arial", new FontResource("gfx\\fonts\\arial.ttf"));
		resourceManager->AddResource("font_copacetix", new FontResource("gfx\\fonts\\copacetix.ttf", 64));
		
		// UI Sounds
		resourceManager->AddResource("menu_click", new SoundResource("sfx\\menu_click.wav"));

		// Set default rendering font
		Renderer::Get()->SetDefaultFont(resourceManager->GetFont("font_arial"));

	}
	else if (gameState == GameState_Game)
	{

		//
		// These resources are loaded when a battle starts
		//

		// UI Images
		resourceManager->AddResource("ui_timer", new ImageResource("gfx\\ui\\timer.tga"));
		resourceManager->AddResource("ui_windbar_empty", new ImageResource("gfx\\ui\\windbar_empty.tga"));
		resourceManager->AddResource("ui_windbar_full", new ImageResource("gfx\\ui\\windbar_full.tga"));
		resourceManager->AddResource("ui_teambar_empty", new ImageResource("gfx\\ui\\teambar_empty.tga"));
		resourceManager->AddResource("ui_teambar_full", new ImageResource("gfx\\ui\\teambar_full.tga"));

		// Team and name texts
		resourceManager->AddResource("text_teamnames", new TextResource("data\\teamnames.txt"));
		resourceManager->AddResource("text_slugnames", new TextResource("data\\slugnames.txt"));\
		resourceManager->AddResource("text_colors", new TextResource("data\\colors.txt"));

		// Images
		resourceManager->AddResource("image_gravestone", new ImageResource("gfx\\graves\\gravestone16x16.png"));
		resourceManager->AddResource("image_water0", new ImageResource("gfx\\levels\\test\\water.tga"));
		resourceManager->AddResource("image_backgroundfar", new ImageResource("gfx\\levels\\test\\back2.png"));
		resourceManager->AddResource("image_backgroundnear", new ImageResource("gfx\\levels\\test\\back1.png"));
		resourceManager->AddResource("image_crosshair", new ImageResource("gfx\\levels\\test\\crosshair.tga"));
		resourceManager->AddResource("image_cloud", new ImageResource("gfx\\clouds\\standard\\smallcloud0.png"));
		resourceManager->AddResource("tb_ground", new TextureBuffer("gfx\\levels\\test\\ground_ice.tga"));
		resourceManager->AddResource("tb_over", new TextureBuffer("gfx\\levels\\test\\over_ice.tga"));
		resourceManager->AddResource("tb_under", new TextureBuffer("gfx\\levels\\test\\under_ice.tga"));
		resourceManager->AddResource("image_slug_left", new ImageResource("gfx\\placeholders\\slug_ph_left.tga"));
		resourceManager->AddResource("image_slug_right", new ImageResource("gfx\\placeholders\\slug_ph_right.tga"));
		resourceManager->AddResource("image_rocket", new ImageResource("gfx\\placeholders\\rocket_ph.tga"));
		resourceManager->AddResource("image_grenade", new ImageResource("gfx\\placeholders\\grenade_ph.tga"));
		resourceManager->AddResource("image_mine", new ImageResource("gfx\\placeholders\\mine_ph.tga"));
		resourceManager->AddResource("particle_explosion", new ImageResource("gfx\\misc\\explosion0.tga"));

	}

}

void Game::ChangeGameState(GameState gameState)
{

	LoadResourcesForState(gameState);

	switch (gameState)
	{

	case GameState_Game:

		CreateWorld();

		break;

	}

	state = gameState;

}

void Game::CreateWorld()
{

	SafeDelete(world);
	world = new World();

	ResourceManager* resources = ResourceManager::Get();

	world->Build(2, 2, (TextureBuffer*)resources->GetResource("tb_ground"), (TextureBuffer*)resources->GetResource("tb_over"), (TextureBuffer*)resources->GetResource("tb_under"), (ImageResource*)resources->GetResource("image_water0"));
	world->SetBackground((ImageResource*)resources->GetResource("image_backgroundfar"), (ImageResource*)resources->GetResource("image_backgroundnear"));

	// Center the camera in the world
	camera->SetPosition(Vec2f((float)world->WidthInPixels() / 2.0f, (float)world->HeightInPixels() / 2.0f));

	// Notify the world that the camera moved (updates parallax)
	world->CameraMoved(camera->GetPosition());

	// Create 4 players
	Player* player = new Player("Player");
	AIPlayer* computer0 = new AIPlayer("Computer0");
	AIPlayer* computer1 = new AIPlayer("Computer1");
	AIPlayer* computer2 = new AIPlayer("Computer2");

	// Create 4 teams
	Team* teamA = new Team(player);
	teamA->Randomize();

	Team* teamB = new Team(computer0);
	teamB->Randomize();

	Team* teamC = new Team(computer1);
	teamC->Randomize();

	Team* teamD = new Team(computer2);
	teamD->Randomize();

	// Assign teams to players
	player->AddTeam(teamA);
	computer0->AddTeam(teamB);
	computer1->AddTeam(teamC);
	computer2->AddTeam(teamD);

	// Place teams in world
	player->PlaceInWorld();
	computer0->PlaceInWorld();
	computer1->PlaceInWorld();
	computer2->PlaceInWorld();

	players.push_back(player);
	players.push_back(computer0);
	players.push_back(computer1);
	players.push_back(computer2);

	// Set active player and start the game
	activePlayer = Random::RandomInt(0, players.size() - 1);
	players[activePlayer]->TurnBegins();

}

Camera* Game::GetCamera() const
{

	return camera;

}

bool Game::GetGameBool(GameBool flag) const
{

	return gameBools[flag];

}

float Game::GetGameFloat(GameFloat flag) const
{

	return gameFloats[flag];

}

int Game::GetGameInt(GameInt flag) const
{

	return gameInts[flag];

}

void Game::SetGameBool(GameBool flag, bool value)
{

	gameBools[flag] = value;

}

void Game::ToggleGameBool(GameBool flag)
{

	gameBools[flag] = !gameBools[flag];

}

void Game::SetGameFloat(GameFloat flag, float value)
{

	gameFloats[flag] = value;

}

void Game::SetGameInt(GameInt flag, int value)
{

	gameInts[flag] = value;

}

bool Game::GetGameDefaultBool(GameBool flag) const
{

	switch (flag)
	{

	case GameBool_CanChooseSlug:
		return false;

	case GameBool_CanUseMultipleSlugsPerTurn:
		return false;

	case GameBool_AlwaysShowNames:
		return true;

	case GameBool_AlwaysShowHps:
		return true;

	case GameBool_Debug:
		return false;

	case GameBool_LockCameraToLevel:
		return true;

	case GameBool_Pause:
		return false;

	case GameBool_FallDamage:
		return true;

	case GameBool_DamageEndsTurn:
		return true;

	case GameBool_DebugTerrain:
		return false;

	}

	ASSERTMSG(0, "No default value for GameBool!");
	return false;

}

float Game::GetGameDefaultFloat(GameFloat flag) const
{

	switch (flag)
	{

	case GameFloat_TurnTime:
		return 30.0f;

	case GameFloat_TurnEndTime:
		return 3.0f;

	}

	ASSERTMSG(0, "No default value for GameFloat!");
	return 0.0f;

}

int Game::GetGameDefaultInt(GameInt flag) const
{

	switch (flag)
	{

	case GameInt_SlugsPerTeam:
		return 5;

	case GameInt_ShotsPerTurn:
		return 1;

	}

	ASSERTMSG(0, "No default value for GameInt!");
	return 0;

}

void Game::EndTurn()
{

	ASSERT(activePlayer != -1);

	// End current players turn
	players[activePlayer]->TurnEnds();

	// Search for the next player with slugs remaining and start their turn
	bool foundNextPlayer = false;

	while (!foundNextPlayer)
	{

		activePlayer ++;

		if (activePlayer == players.size())
			activePlayer = 0;

		if (players[activePlayer]->HasAliveSlugs())
			foundNextPlayer = true;

	}

	players[activePlayer]->TurnBegins();

}

Player* Game::GetCurrentPlayer() const
{

	if ((activePlayer < 0) || (activePlayer >= players.size()))
		return NULL;

	return players[activePlayer];

}

bool Game::EnsureNoAction() const
{

	return noAction;

}

bool Game::IsActivePlayer(Player* player) const
{

	return (player == players[activePlayer]);

}