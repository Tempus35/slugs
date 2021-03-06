#include "game.h"
#include "player.h"
#include "aiplayer.h"

Game::Game()
{

	world = NULL;
	updateManager = new UpdateManager();
	fxManager = new FXManager();
	uiManager = new UIManager();

	for (int i = 0; i < GameBool_LAST; ++ i)
		gameBools.push_back(GetGameDefaultBool((GameBool)i));

	for (int i = 0; i < GameFloat_LAST; ++ i)
		gameFloats.push_back(GetGameDefaultFloat((GameFloat)i));

	for (int i = 0; i < GameInt_LAST; ++ i)
		gameInts.push_back(GetGameDefaultInt((GameInt)i));

	state = GameState_None;

	camera = new Camera();

	noAction = true;
	loading = false;

	// Load initial resources
	LoadResourcesForState(GameState_None);

}

Game::~Game()
{

	for (unsigned int i = 0; i < players.size(); ++ i)
		SafeDelete(players[i]);
	
	SafeDelete(camera);

	while (world->IsBuilding())
	{

		world->CancelBuilding();
		Sleep(10);

	}

	SafeDelete(world);
	
	SafeDelete(updateManager);
	SafeDelete(fxManager);

}

void Game::Update(float elapsedTime)
{

	// Update the UI
	uiManager->Update(elapsedTime);

	if (loading)
	{

		if (state == GameState_Build)
		{

			// We just finished building the world, advance to the game state
			if (!world->IsBuilding())
				ResourcesLoaded();

		}
		else
		{

			if (!ResourceManager::Get()->IsLoading())
				ResourcesLoaded();

		}

	}
	else
	{

		if ((state == GameState_Game) || (state == GameState_MainMenu))
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

	}

	// Update renderer
	Renderer::Get()->Update(elapsedTime);

}

void Game::Render()
{

	if (loading)
	{

		//
		// Render the loading screen
		//

		Renderer::Get()->SetCamera(NULL);

		uiManager->Render();

	}
	else
	{

		// Set the camera
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

				char text[1024];

				Vec2f worldCursorPos = camera->GetWorldPosition(cursorPosition.x, cursorPosition.y);
				sprintf_s(text, 1024, "%.0f, %.0f", worldCursorPos.x, worldCursorPos.y);

				Renderer::Get()->DrawDebugHint(worldCursorPos + Vec2f(20.0f, +50.0f), text, Color(255, 255, 255));

				if (object)
				{

					ObjectType objectType = object->GetType();
					const Vec2f& objectPos = object->GetPosition();
					const Vec2f& objectVel = object->GetVelocity();
	
					sprintf_s(text, 1024, "[%s] (%i hps)\n\nPosition: %.1f, %.1f\nVelocity: %.1f, %.1f (%.1f)\n\nHeight: %.0f\nAtRest = %s", ObjectTypeToString(objectType).c_str(), object->GetHitPoints(), objectPos.x, objectPos.y, objectVel.x, objectVel.y, object->GetSpeed(), object->GetHeightAboveTerrain(), BoolToString(object->IsAtRest()).c_str());
					
					if (objectType == ObjectType_Projectile)
						sprintf_s(&text[strlen(text)], 1024 - strlen(text), "\n\nTimer: %.1f", ((Projectile*)object)->GetTimer());
					
					Renderer::Get()->DrawDebugHint(objectPos + Vec2f(20.0f, -20.0f), text, Color(255, 255, 255));

				}

			}

		#endif

		// Render world space debugging overlay
		Renderer::Get()->DebugDraw();

		// Turn the camera off for screen space mode
		Renderer::Get()->SetCamera(NULL);

		//
		// Render UI
		// TODO: This eventually needs to move behind the screen space debug layer when it is cleared in favor of ui controls
		//

		uiManager->Render();

		//
		// PostFX
		//

		Renderer::Get()->RenderPostFX();

		//
		// Screen space debugging overlay
		//

		UIConsole* console = (UIConsole*)uiManager->GetWidget("console");

		if ((console == NULL) || (!console->IsOpen()))
		{

			Player* currentPlayer = GetCurrentPlayer();

			if ((currentPlayer) && (currentPlayer->GetCurrentSlug()))
			{

				char debugInfo[1024];
				sprintf_s(debugInfo, 1024, "%s - %s", currentPlayer->GetName().c_str(), currentPlayer->GetCurrentSlug()->GetName().c_str());
				Renderer::Get()->RenderText(10, 40, NULL, debugInfo, 16.0f, Color(255, 255, 255));

				if (GetGameBool(GameBool_Debug))
				{

					float power = currentPlayer->GetCurrentSlug()->GetPower() * 100.0f;

					if (power > 0.0f)
					{

						Vec2i powerPos = Renderer::Get()->GetRelativeCoordinate(0, 35, RenderPosition_Bottom);
						sprintf_s(debugInfo, 1024, "%.0f%%", power);
						Renderer::Get()->RenderText((float)powerPos.x, (float)powerPos.y, NULL, debugInfo, 30.0f, Color(255, 255, 255), FontFlag_Centered);

					}

				}

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

		if (state == GameState_MainMenu)
		{

			Vec2i textPos = Renderer::Get()->GetRelativeCoordinate(0, 100, RenderPosition_Bottom);
			Renderer::Get()->RenderText((float)textPos.x, (float)textPos.y, ResourceManager::Get()->GetFont("font_copacetix"), "Press any key.", 32.0f, Color::green, FontFlag_Bold|FontFlag_Centered);

		}

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

UIManager* Game::GetUIManager() const
{

	return uiManager;

}

bool Game::KeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	// UI gets first shot at the event
	if (uiManager->KeyDown(key, shift, control, alt))
		return true;

	if (state == GameState_MainMenu)
	{

		ResourcesLoaded();

	}
	else if (state == GameState_Game)
	{

		Slug* selectedSlug;
		bool turnEnding = GetCurrentPlayer()->IsTurnEnding();
		bool paused = GetGameBool(GameBool_Pause);

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
			if (world->SelectedObject())
				world->SelectedObject()->SetHitpoints(-1);
			
			break;

		case sf::Key::D:

			// Toggle debug overlay
			ToggleGameBool(GameBool_Debug);

			break;

		case sf::Key::E:

			// Force the current players turn to end
			Game::EndTurn();

			break;

		case sf::Key::C:

			// Drop a crate
			Game::Get()->GetWorld()->DropCrate();

			break;

		case sf::Key::S:

			// Simulate an explosion
			world->SimulateExplosion(camera->GetWorldPosition(cursorPosition.x, cursorPosition.y), ExplosionData(100.0f, 125.0f, 500.0f, 100.0f, 0.0f));

			break;

		#endif

		case sf::Key::Tilde:

			// Open or close the console
			((UIConsole*)Game::Get()->GetUIManager()->GetWidget("console"))->Toggle();

			break;

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

			if ((selectedSlug) && (!paused))
				selectedSlug->StartMovingLeft();

			break;

		case sf::Key::Right:

			if ((selectedSlug) && (!paused))
				selectedSlug->StartMovingRight();

			break;

		case sf::Key::Up:

			if ((selectedSlug) && (!paused))
				selectedSlug->StartAimingUp();

			break;

		case sf::Key::Down:
			
			if ((selectedSlug) && (!paused))
				selectedSlug->StartAimingDown();

			break;

		case sf::Key::Return:

			if ((selectedSlug) && (!paused))
				selectedSlug->Jump();

			break;

		case sf::Key::Space:

			if ((selectedSlug) && (!turnEnding) && (!paused))
				selectedSlug->StartCharging();

			break;

		case sf::Key::Num1:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Bazooka);

			break;

		case sf::Key::Num2:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Grenade);

			break;

		case sf::Key::Num3:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Shotgun);

			break;

		case sf::Key::Num4:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Machinegun);

			break;

		case sf::Key::Num5:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Mine);

			break;

		case sf::Key::Num6:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_HomingMissile);

			break;

		case sf::Key::Num7:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Dynamite);

			break;

		case sf::Key::Num8:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Clusterbomb);

			break;

		case sf::Key::Num9:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Airstrike);

			break;

		case sf::Key::Num0:

			if ((selectedSlug) && (!paused))
				selectedSlug->ArmSelf(WeaponType_Teleporter);

			break;

		}

	}

	return true;

}

bool Game::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	// UI gets first shot at the event
	if (uiManager->KeyUp(key, shift, control, alt))
		return true;

	if (state == GameState_Game)
	{
		
		Slug* selectedSlug;
		bool turnEnding = GetCurrentPlayer()->IsTurnEnding();
		bool paused = GetGameBool(GameBool_Pause);

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

				if ((selectedSlug) && (!paused) && (!turnEnding))
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

		// Loading graphic
		// This is loaded on the main thread and added to the resource manager since we need it to display the loading screen
		resourceManager->AddResource("image_loading", new ImageResource("gfx\\placeholders\\loadingpie_ph.tga"));
		resourceManager->AddResource("font_arial", new FontResource("gfx\\fonts\\arial.ttf"));

		// Show the loading screen
		SetLoading(true);

		// Fonts
		resourceManager->QueueResource("font_copacetix", ResourceType_Font, "gfx\\fonts\\copacetix.ttf");
		
		// UI Sounds
		resourceManager->QueueResource("menu_click", ResourceType_Sound, "sfx\\menu_click.wav");

		// UI Images
		resourceManager->QueueResource("ui_turntimer", ResourceType_Image, "gfx\\ui\\timer.tga");
		resourceManager->QueueResource("ui_chargebar_bg", ResourceType_Image, "gfx\\ui\\chargebar_bg.tga");
		resourceManager->QueueResource("ui_chargebar_fill", ResourceType_Image, "gfx\\ui\\chargebar_fill.tga");
		resourceManager->QueueResource("ui_windbar_empty", ResourceType_Image, "gfx\\ui\\windbar_empty.tga");
		resourceManager->QueueResource("ui_windbar_full", ResourceType_Image, "gfx\\ui\\windbar_full.tga");
		resourceManager->QueueResource("ui_teambar_empty", ResourceType_Image, "gfx\\ui\\teambar_empty.tga");
		resourceManager->QueueResource("ui_teambar_full", ResourceType_Image, "gfx\\ui\\teambar_full.tga");

		// Team and name texts
		resourceManager->QueueResource("text_teamnames", ResourceType_Text, "data\\teamnames.txt");
		resourceManager->QueueResource("text_slugnames", ResourceType_Text, "data\\slugnames.txt");
		resourceManager->QueueResource("text_colors", ResourceType_Text, "data\\colors.txt");

		// Ambient
		resourceManager->QueueResource("image_cloud", ResourceType_Image, "gfx\\clouds\\standard\\smallcloud0.png");
		resourceManager->QueueResource("image_water0", ResourceType_Image, "gfx\\levels\\test\\water.tga");
		resourceManager->QueueResource("image_backgroundfar", ResourceType_Image, "gfx\\levels\\test\\back2.png");
		resourceManager->QueueResource("image_backgroundnear", ResourceType_Image, "gfx\\levels\\test\\back1.png");

		// Flavor objects
		resourceManager->QueueResource("image_gravestone", ResourceType_Image, "gfx\\graves\\gravestone16x16.png");
		
		// Ground
		resourceManager->QueueResource("tb_ground", ResourceType_Buffer, "gfx\\levels\\test\\ground_ice.tga");
		resourceManager->QueueResource("tb_over", ResourceType_Buffer, "gfx\\levels\\test\\over_ice.tga");
		resourceManager->QueueResource("tb_under", ResourceType_Buffer, "gfx\\levels\\test\\under.tga");
		resourceManager->QueueResource("tb_logo", ResourceType_Buffer, "gfx\\misc\\slugs.tga");

		// Slug
		resourceManager->QueueResource("image_crosshair", ResourceType_Image, "gfx\\levels\\test\\crosshair.tga");
		resourceManager->QueueResource("image_slug_left", ResourceType_Image, "gfx\\placeholders\\slug_ph_left.tga");
		resourceManager->QueueResource("image_slug_right", ResourceType_Image, "gfx\\placeholders\\slug_ph_right.tga");

		// Projectiles
		resourceManager->QueueResource("image_rocket", ResourceType_Image, "gfx\\placeholders\\rocket_ph.tga");
		resourceManager->QueueResource("image_grenade", ResourceType_Image, "gfx\\placeholders\\grenade_ph.tga");
		resourceManager->QueueResource("image_mine", ResourceType_Image, "gfx\\placeholders\\mine_ph.tga");
		resourceManager->QueueResource("image_dynamite", ResourceType_Image, "gfx\\placeholders\\dynamite_ph.tga");

		// Pickups
		resourceManager->QueueResource("image_crate", ResourceType_Image, "gfx\\placeholders\\crate_ph.tga");

		// Particles
		resourceManager->QueueResource("particle_explosion", ResourceType_Image, "gfx\\misc\\explosion0.tga");

		// Hats
		resourceManager->QueueResource("image_hat0", ResourceType_Image, "gfx\\placeholders\\hat_ph.tga");

		// Load 'em
		resourceManager->LoadQueuedResources();

	}

}

void Game::ResourcesLoaded()
{

	if (state == GameState_None)
	{

		// Set default rendering font
		Renderer::Get()->SetDefaultFont(ResourceManager::Get()->GetFont("font_arial"));

		// Add a console to the UI
		UIConsole* console = new UIConsole("console");
		uiManager->AddWidget(console);

		// Register console commands
		RegisterConsoleCommands();

		// Advance to the main menu
		ChangeGameState(GameState_MainMenu);

		CreateMainMenu();

		SetLoading(false);

	}
	else if (state == GameState_MainMenu)
	{

		SetLoading(true);

		// Advance to world building
		ChangeGameState(GameState_Build);

		// Create the world
		CreateWorld();

	}
	else if (state == GameState_Build)
	{

		SetupWorld();

		ChangeGameState(GameState_Game);
	
	}
	else if (state == GameState_Game)
	{

		//
		// Add UI elements
		//
		
		// Turn timer
		UITimer* timer = new UITimer("turntimer", UIGroup_Game, ResourceManager::Get()->GetImage("ui_turntimer"), ResourceManager::Get()->GetFont("font_copacetix"));
		timer->SetPosition(Renderer::Get()->GetRelativeCoordinate(64, 64, RenderPosition_BottomRight));
		uiManager->AddWidget(timer);

		// Charge indicator
		UIChargebar* chargeBar = new UIChargebar("chargebar", UIGroup_Game, ResourceManager::Get()->GetImage("ui_chargebar_bg"), ResourceManager::Get()->GetImage("ui_chargebar_fill"));
		chargeBar->SetPosition(Renderer::Get()->GetRelativeCoordinate(0, 64, RenderPosition_Bottom));
		uiManager->AddWidget(chargeBar);

		SetLoading(false);

	}

}

void Game::ChangeGameState(GameState gameState)
{

	LoadResourcesForState(gameState);

	state = gameState;

}

GameState Game::GetGameState() const
{

	return state;

}

void Game::CreateMainMenu()
{

	SafeDelete(world);
	world = new World();

	ResourceManager* resources = ResourceManager::Get();

	world->Build(2, 2, false);
	world->GetTerrain()->AddImage(resources->GetBuffer("tb_logo"), 1024, 1024);

	// Center the camera in the world
	camera->SetPosition(Vec2f((float)world->WidthInPixels() / 2.0f, (float)world->HeightInPixels() / 2.0f));

	// Notify the world that the camera moved (updates parallax)
	world->CameraMoved(camera->GetPosition());

	world->GetTerrain()->BufferAll();

	AIPlayer* cpu0 = new AIPlayer("cpu0");
	AIPlayer* cpu1 = new AIPlayer("cpu1");
	Team* cpu0t = new Team(cpu0);
	cpu0t->Randomize(1);
	Team* cpu1t = new Team(cpu1);
	cpu1t->Randomize(1);
	cpu0->AddTeam(cpu0t);
	cpu1->AddTeam(cpu1t);
	cpu0->PlaceInWorld();
	cpu1->PlaceInWorld();

	players.push_back(cpu0);
	players.push_back(cpu1);
	activePlayer = 0;
	players[activePlayer]->TurnBegins();

}

void Game::CreateWorld()
{

	SafeDelete(world);
	world = new World();

	ResourceManager* resources = ResourceManager::Get();

	world->Build(2, 2, true, resources->GetBuffer("tb_ground"), resources->GetBuffer("tb_over"), resources->GetBuffer("tb_under"), (ImageResource*)resources->GetResource("image_water0"));
	world->SetBackground((ImageResource*)resources->GetResource("image_backgroundfar"), (ImageResource*)resources->GetResource("image_backgroundnear"));

	// Center the camera in the world
	camera->SetPosition(Vec2f((float)world->WidthInPixels() / 2.0f, (float)world->HeightInPixels() / 2.0f));

	// Notify the world that the camera moved (updates parallax)
	world->CameraMoved(camera->GetPosition());

}

void Game::SetupWorld()
{

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
		return true;

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

	case GameFloat_CrateDropChance:
		return 0.1f;

	case GameFloat_WeaponChargeRate:
		return 0.5f;

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

	case GameInt_SlugHitpoints:
		return 100;

	case GameInt_HealthPickupAmount:
		return 50;

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

	// Determine random events
	float crateDrop = Random::RandomFloat(0.0f, 1.0f);
	
	if (crateDrop < GetGameFloat(GameFloat_CrateDropChance))
		world->DropCrate();

	// Start the turn
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

void Game::SetLoading(bool state)
{

	if (loading != state)
	{

		loading = state;

		if (loading)
		{
		
			uiManager->RemoveWidgetsInGroup(UIGroup_Other);

			UILoadGraphic* graphic = new UILoadGraphic(ResourceManager::Get()->GetImage("image_loading"), "graphic_loading", UIGroup_Other);
			graphic->SetPosition(Vec2i(Renderer::Get()->GetWidth() / 2, Renderer::Get()->GetHeight() / 2));

			uiManager->AddWidget(graphic);

		}
		else
		{

			uiManager->RemoveWidgetsInGroup(UIGroup_Other);
			Renderer::Get()->SetFade(1.0f);

		}

	}
	else
	{

		ASSERT(0);

	}

}

void Game::RegisterConsoleCommands()
{

	UIConsole* console = (UIConsole*)uiManager->GetWidget("console");

	if (console)
	{

		console->Register("debug", new ConsoleCommand_0<Game>("Toggles debugging mode.", this, &Game::ConsoleCallbackToggleDebug));
		console->Register("game.turntime", new ConsoleCommand_1<Game, float>("Sets the time in seconds per turn.", this, &Game::ConsoleCallbackSetTurnTime));
		console->Register("game.cratedropchance", new ConsoleCommand_1<Game, float>("Sets the chance of a crate drop occuring per turn (0.0 to 1.0)", this, &Game::ConsoleCallbackSetCrateDropChance));
		console->Register("game.weaponchargerate", new ConsoleCommand_1<Game, float>("Sets the rate of weapon charge in units/second (weapons fire at charge = 1.0)", this, &Game::ConsoleCallbackSetWeaponChargeRate));

	}

}

void Game::ConsoleCallbackToggleDebug()
{

	ToggleGameBool(GameBool_Debug);

}

void Game::ConsoleCallbackSetTurnTime(float v)
{

	SetGameFloat(GameFloat_TurnTime, v);

}

void Game::ConsoleCallbackSetCrateDropChance(float v)
{

	SetGameFloat(GameFloat_CrateDropChance, v);

}

void Game::ConsoleCallbackSetWeaponChargeRate(float v)
{

	SetGameFloat(GameFloat_WeaponChargeRate, v);

}

void Game::Log(LogType type, const char* format, ...)
{

	va_list args;
	va_start(args, format);

	if (GetGameBool(GameBool_Debug))
	{

		UIConsole* console = (UIConsole*)uiManager->GetWidget("console");

		if (console)
		{

			char buffer[1024];
			vsprintf_s(buffer, 1024, format, args);

			console->Print(buffer);

		}

	}

	Log::Get()->Write(type, format, args);

	va_end(args);

}