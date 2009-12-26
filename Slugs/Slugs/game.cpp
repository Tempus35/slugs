#include "game.h"
#include "player.h"
#include "aiplayer.h"

Game::Game()
{

	world = NULL;
	updateManager = new UpdateManager();
	fxManager = new FXManager();

	for (int i = 0; i < GameFlag_LAST; ++ i)
		flags.push_back(true);

	lockCameraToLevel = true;

	state = GameState_None;

	camera = new Camera();

}

Game::~Game()
{

	std::list<Player*>::iterator i = players.begin();

	while (i != players.end())
	{

		SafeDelete(*i);
		++i;

	}

	SafeDelete(camera);
	SafeDelete(world);
	SafeDelete(updateManager);
	SafeDelete(fxManager);

}

void Game::Update(float elapsedTime)
{

	// Update the camera
	if (camera->Update(elapsedTime))
		world->CameraMoved(camera->GetPosition());

	// Update the update manager
	if (updateManager)
		updateManager->Update(elapsedTime);

	// Update world
	if (world)
		world->Update(elapsedTime);

	if (fxManager)
		fxManager->Update(elapsedTime);

}

void Game::Render()
{

	Renderer::Get()->SetCamera(camera);

	if (world)
	{

		world->Render();

	}

	Renderer::Get()->DebugDraw();

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

bool Game::IsFlagSet(GameFlag flag) const
{

	return flags[flag];

}

bool Game::KeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (state == GameState_Game)
	{

		Object* selectedObject = world->SelectedObject();

		switch (key)
		{

		//
		// Debugging
		//

		case sf::Key::G:
			world->Regenerate();
			break;

		case sf::Key::L:
			lockCameraToLevel = !lockCameraToLevel;
			break;

		case sf::Key::K:

			if (selectedObject)
				selectedObject->SetHitpoints(-1);
			
			break;

		//
		// Team Controls
		//

		case sf::Key::Tab:

			if (IsFlagSet(GameFlag_CanChooseSlug))
				(*(players.begin()))->SelectNextSlug();

			break;
	
		//
		// Slug Controls
		//

		case sf::Key::Left:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->StartMovingLeft();

			break;

		case sf::Key::Right:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->StartMovingRight();

			break;

		case sf::Key::Up:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->StartAimingUp();

			break;

		case sf::Key::Down:
			
			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->StartAimingDown();

			break;

		case sf::Key::Return:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->Jump();

			break;

		case sf::Key::Space:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->StartCharging();

			break;

		case sf::Key::Num1:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->ArmSelf(WeaponType_Bazooka);

			break;

		case sf::Key::Num2:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->ArmSelf(WeaponType_Grenade);

			break;

		case sf::Key::Num3:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->ArmSelf(WeaponType_Shotgun);

			break;

		case sf::Key::Num4:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->ArmSelf(WeaponType_Machinegun);

			break;

		case sf::Key::Num5:

			if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
				((Slug*)selectedObject)->ArmSelf(WeaponType_Mine);

			break;

		}

	}

	return true;

}

bool Game::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (state == GameState_Game)
	{
		
		Object* selectedObject = Game::Get()->GetWorld()->SelectedObject();
		
		switch (key)
		{

			case sf::Key::Left:
			case sf::Key::Right:

				if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
					((Slug*)selectedObject)->StopMoving();
			
				break;

			case sf::Key::Up:
			case sf::Key::Down:

				if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
					((Slug*)selectedObject)->StopAiming();

				break;

			case sf::Key::Space:

				if ((selectedObject) && (selectedObject->GetType() == ObjectType_Slug))
					((Slug*)selectedObject)->Fire();

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
						
			Vec2f pickPosition = camera->GetWorldPosition(position.x, position.y);
			Object* pickedObject = Game::Get()->GetWorld()->GetObjectAtPosition(pickPosition);

			if (pickedObject)
				world->SelectObject(pickedObject);
			//else
			//	world->SimulateExplosion(pickPosition.x, pickPosition.y, Random::RandomFloat(30.0f, 100.0f));

		}

	}

	return true;

}

bool Game::MouseUp(const Vec2i& position, sf::Mouse::Button button)
{

	return true;

}

bool Game::MouseMoved(bool left, bool right, bool middle, const Vec2i& from, const Vec2i& to)
{

	if ((state == GameState_Game) && (right == true))
	{

		//
		// RMB moves the camera
		//

		Vec2i delta = from - to;

		sf::Rect<float> viewRect = camera->GetView().GetRect();

		if (lockCameraToLevel)
		{

			if (delta.x < 0)
			{

				if (-delta.x > (int)viewRect.Left)
					delta.x = -(int)viewRect.Left; 

			}
			else
			{

				int d = Game::Get()->GetWorld()->WidthInPixels() - (int)viewRect.Right;
				
				if (d < delta.x)
					delta.x = d;

			}

			if (delta.y < 0)
			{

				int d = -Game::Get()->GetWorld()->HeightInPixels() - (int)viewRect.Top;

				if (d > delta.y)
					delta.y = d;

			}
			else
			{

				if (delta.y > -(int)viewRect.Bottom)
					delta.y = -(int)viewRect.Bottom;

			}

		}

		camera->Move(delta.x, -delta.y);

		// Lets the world know the camera moved
		world->CameraMoved(camera->GetPosition());

	}

	return true;

}

void Game::LoadResourcesForState(GameState gameState)
{

	ResourceManager* resourceManager = ResourceManager::Get();

	if (gameState == GameState_Game)
	{

		//
		// Build debugging text resources
		//

		std::vector<std::string> names;

		names.push_back("Read Team"); names.push_back("Green Team"); names.push_back("Blue Team"); names.push_back("Yellow Team");
		names.push_back("Pink Team"); names.push_back("Purple Team"); names.push_back("White Team"); names.push_back("Black Team");
		names.push_back("Orange Team");
		resourceManager->AddResource("text_teamnames", new TextResource(names));
		names.clear();

		names.push_back("Paul"); names.push_back("Tim"); names.push_back("Andreas"); names.push_back("Bob"); names.push_back("George");
		names.push_back("Texas"); names.push_back("Pete"); names.push_back("Adam"); names.push_back("Angela"); names.push_back("Richard");
		names.push_back("Bill"); names.push_back("Tony"); names.push_back("Travis"); names.push_back("Theodore"); names.push_back("Amy");
		names.push_back("Josie"); names.push_back("Clint"); names.push_back("Chris"); names.push_back("Jesus"); names.push_back("Jose");
		names.push_back("Katie"); names.push_back("Steve"); names.push_back("Sarah");
		resourceManager->AddResource("text_slugnames", new TextResource(names));
		names.clear();

		//
		// Load other resources
		//

		resourceManager->AddResource("image_gravestone", new ImageResource("gfx\\graves\\gravestone16x16.png"));
		resourceManager->AddResource("image_water0", new ImageResource("gfx\\levels\\test\\water.tga"));
		resourceManager->AddResource("image_backgroundfar", new ImageResource("gfx\\levels\\test\\back2.png"));
		resourceManager->AddResource("image_backgroundnear", new ImageResource("gfx\\levels\\test\\back1.png"));
		resourceManager->AddResource("image_crosshair", new ImageResource("gfx\\levels\\test\\crosshair.tga"));
		resourceManager->AddResource("image_arrow", new ImageResource("gfx\\arrow-sprite.png"));
		resourceManager->AddResource("image_cloud", new ImageResource("gfx\\clouds\\standard\\smallcloud0.png"));
		resourceManager->AddResource("tb_ground", new TextureBuffer("gfx\\levels\\test\\ground.raw", 256, 256, 4));
		resourceManager->AddResource("tb_over", new TextureBuffer("gfx\\levels\\test\\over.raw", 256, 12, 4));
		resourceManager->AddResource("tb_under", new TextureBuffer("gfx\\levels\\test\\under.raw", 256, 4, 4));
		resourceManager->AddResource("image_jumping_right", new ImageResource("gfx\\jumping_right.png"));
		resourceManager->AddResource("image_explosion", new ImageResource("gfx\\SkybusterExplosion.jpg"));
		resourceManager->AddResource("image_options_notselected", new ImageResource("gfx\\menu\\menu_options_not_selected.png"));
		resourceManager->AddResource("image_options_selected", new ImageResource("gfx\\menu\\menu_options_selected.png"));
		resourceManager->AddResource("image_credits_notselected", new ImageResource("gfx\\menu\\menu_credits_not_selected.png"));
		resourceManager->AddResource("image_credits_selected", new ImageResource("gfx\\menu\\menu_credits_selected.png"));
		resourceManager->AddResource("image_SP_not_selected", new ImageResource("gfx\\menu\\menu_SP_not_selected.png"));
		resourceManager->AddResource("image_SP_selected", new ImageResource("gfx\\menu\\menu_SP_selected.png"));
		resourceManager->AddResource("image_MP_not_selected", new ImageResource("gfx\\menu\\menu_MP_not_selected.png"));
		resourceManager->AddResource("image_MP_selected", new ImageResource("gfx\\menu\\menu_MP_selected.png"));
		resourceManager->AddResource("image_menu_background", new ImageResource("gfx\\menu\\menu_background.png"));
		resourceManager->AddResource("image_logo", new ImageResource("gfx\\2uoo2ti.png"));
		resourceManager->AddResource("image_slug_left", new ImageResource("gfx\\slug_ph_left.tga"));
		resourceManager->AddResource("image_slug_right", new ImageResource("gfx\\slug_ph_right.tga"));
		resourceManager->AddResource("image_rocket", new ImageResource("gfx\\rocket_ph.tga"));
		resourceManager->AddResource("image_grenade", new ImageResource("gfx\\grenade_ph.tga"));
		resourceManager->AddResource("image_mine", new ImageResource("gfx\\mine_ph.tga"));
		resourceManager->AddResource("particle_explosion", new ImageResource("gfx\\explosion0.tga"));
		resourceManager->AddResource("image_snowflake", new ImageResource("gfx\\snowflake.tga"));
		resourceManager->AddResource("font_arial", new FontResource("gfx\\fonts\\arial.ttf"));
		resourceManager->AddResource("font_copacetix", new FontResource("gfx\\fonts\\copacetix.ttf", 64));

		resourceManager->AddResource("menu_click", new SoundResource("sfx\\menu_click.WAV"));

		TextureBuffer* rawTex[3];
		rawTex[0] = new TextureBuffer("gfx\\levels\\test\\ground.raw", 256, 256, 4);
		resourceManager->AddResource("test_ground", rawTex[0]);
		rawTex[1] = new TextureBuffer("gfx\\levels\\test\\over.raw", 256, 12, 4);
		resourceManager->AddResource("test_over", rawTex[1]);
		rawTex[2] = new TextureBuffer("gfx\\levels\\test\\under.raw", 256, 4, 4);
		resourceManager->AddResource("test_under", rawTex[2]);

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
	lockCameraToLevel = true;

	// Notify the world that the camera moved (updates parallax)
	world->CameraMoved(camera->GetPosition());

	// Create 2 players
	Player* player = new Player();
	AIPlayer* computer = new AIPlayer();

	// Create 2 teams
	Team* teamA = new Team();
	teamA->Randomize();

	Team* teamB = new Team();
	teamB->Randomize();

	// Assign teams to players
	player->AddTeam(teamA);
	computer->AddTeam(teamB);

	// Place teams in world
	player->PlaceInWorld();
	computer->PlaceInWorld();

	players.push_back(player);
	players.push_back(computer);

}

Camera* Game::GetCamera() const
{

	return camera;

}