#include "game.h"

cSlugs::cSlugs() {
	bPushed = false;

	//change scene here 
	iScene = SCENE_ENGINE_TEST;  //SCENE_LEVEL_TEST

	bFullscreen = false;
	iGroundObject = 0;
	iScreenWidth = 1024;
	iScreenHeight = 768;
	bPushedLeft = false;
	iPlayerIndex = 0;
	bRunning = true;
	frame = 0;
	fps = 0;
	time = 0.0f;
	clock.Reset();

	rmbDown = false;
	lockCameraToLevel = true;
}

cSlugs::~cSlugs() {

	Shutdown();

}

void cSlugs::UpdateFPS(float elapsedTime)
{

	if (elapsedTime < 1.0f)
	{

		frame++;
		time += elapsedTime;

		if (time > 1e-6f)
			fps = (float)frame / time;
		else
			fps = 0;

	}

}

void cSlugs::bUpdate()
{

	Renderer* renderer = Renderer::Get();

	// Game Loop	
	float elapsedTime;

	while (bRunning)
	{
	
		// Get time since last frame
		elapsedTime = clock.GetElapsedTime();

		// Update FPS counter
		UpdateFPS(elapsedTime);

		// Reset clock
		clock.Reset();

		// Process SFML events
		sf::Event Event;
		while (renderer->GetWindow().GetEvent(Event))
		{
			
			if (!bInput(&Event)) 
				bRunning = false;

		}

		// Update
		UpdateScene(elapsedTime);
			
		// Main render loop
		bRender();

	}
}

void cSlugs::UpdateScene(float elapsedTime)
{

	switch (iScene)
	{

		case SCENE_LEVEL_TEST:

#ifdef CAMERA_EDGE_MOVE

			bool camMoved = false;

			// Camera movement when mouse is at edge of screen
			if (!lockCameraToLevel)
			{

				// Free movement
				if (iMouseX < CAMERA_MOVE_BORDER)
				{

					cam->Move(-CAMERA_MOVE_SHIFT, 0);
					camMoved = true;

				}
				else if (iMouseX > iScreenWidth - CAMERA_MOVE_BORDER)
				{
					
					cam->Move(CAMERA_MOVE_SHIFT, 0);
					camMoved = true;

				}

				if (iMouseY < CAMERA_MOVE_BORDER)
				{
					
					cam->Move(0, -CAMERA_MOVE_SHIFT);
					camMoved = true;
				
				}		
				else if (iMouseY > iScreenHeight - CAMERA_MOVE_BORDER)
				{

					cam->Move(0, CAMERA_MOVE_SHIFT);
					camMoved = true;

				}

			}
			else
			{

				// Clamp camera to level bounds
				sf::Rect<float> viewRect = cam->GetRect();
				float d;
				
				if (iMouseX < CAMERA_MOVE_BORDER)
				{

					if (viewRect.Left > 0)
					{

						d = viewRect.Left;
						cam->Move(-min(CAMERA_MOVE_SHIFT, d), 0);
						camMoved = true;

					}

				}
				else if (iMouseX > iScreenWidth - CAMERA_MOVE_BORDER)
				{

					if (viewRect.Right < world->WidthInPixels())
					{

						d = (float)world->WidthInPixels() - viewRect.Right;
						cam->Move(min(CAMERA_MOVE_SHIFT, d), 0);
						camMoved = true;

					}

				}

				if (iMouseY < CAMERA_MOVE_BORDER)
				{

					if (viewRect.Top > -world->HeightInPixels())
					{

						d = viewRect.Top + (float)world->HeightInPixels();
						cam->Move(0, -min(CAMERA_MOVE_SHIFT, d));
						camMoved = true;

					}

				}
				else if (iMouseY > iScreenHeight - CAMERA_MOVE_BORDER)
				{

					if (viewRect.Bottom < 0)
					{

						d = -viewRect.Bottom;
						cam->Move(0, min(CAMERA_MOVE_SHIFT, d));
						camMoved = true;

					}

				}

			}

			if (camMoved)
				world->CameraMoved(cam->GetCenter());

#endif

			// Update world
			World::Get()->Update(elapsedTime);

			// Update fx
			FXManager::Get()->Update(elapsedTime);

		break;

	}

}

bool cSlugs::bInput(sf::Event *myEvent) {


	// Check for close events
	if (myEvent->Type == sf::Event::Closed)
		return false;

	// Check for escape key pressed (fixed)
	if ((myEvent->Type == sf::Event::KeyPressed) && (myEvent->Key.Code == sf::Key::Escape))
		return false;

	// Update mouse position if the mouse moved
	if (myEvent->Type == sf::Event::MouseMoved)
	{

		lastMouseX = iMouseX;
		lastMouseY = iMouseY;
		iMouseX = myEvent->MouseMove.X;
		iMouseY = myEvent->MouseMove.Y;

	}

	if ((iScene != SCENE_LEVEL_TEST) && (myEvent->Type == sf::Event::KeyPressed) && (myEvent->Key.Code == sf::Key::P))
	{

		InitLevelTest();
		iScene = SCENE_LEVEL_TEST;

	}

	switch (iScene) {
		case SCENE_LOGO:
			//Controls
			//if (hge->Input_GetKeyState(HGEK_SPACE)) { iScene = SCENE_MENUE; bPushedLeft = true; }
			//if (hge->Input_GetKeyState(HGEK_ENTER)) { iScene = SCENE_MENUE; bPushedLeft = true; }
			//if (hge->Input_GetKeyState(HGEK_LBUTTON)) { iScene = SCENE_MENUE; bPushedLeft = true; }
			break;
		case SCENE_MENUE:
			//if (hge->Input_GetKeyState(HGEK_LEFT)) unitSlugs[iPlayerIndex]->Move(-1, 0);
			//if (hge->Input_GetKeyState(HGEK_RIGHT)) unitSlugs[iPlayerIndex]->Move(+1, 0);
			//if (hge->Input_GetKeyState(HGEK_UP)) unitSlugs[iPlayerIndex]->Rotation((float)+0.1);
			//if (hge->Input_GetKeyState(HGEK_DOWN)) unitSlugs[iPlayerIndex]->Rotation((float)-0.1);
			//if (hge->Input_KeyUp(HGEK_TAB)) {
			//	bPushed = false;
			//}
			//if (hge->Input_KeyDown(HGEK_TAB)) {
			//	if (!bPushed) {
			//		if (iPlayerIndex == 0) iPlayerIndex = 1; else iPlayerIndex = 0;
			//		bPushed = true;
			//	}
			//}

			//if (hge->Input_GetKeyState(HGEK_LBUTTON) && !bPushedLeft) {
			//	if (credits->Collision(fMouseX, fMouseY)) { 
			//		iScene = SCENE_MENUE;
			//		sfx->PlaySnd(false);
			//	}
			//	if (singleplayer->Collision(fMouseX, fMouseY)) {
			//		//bodies clearen
			//		//physic->clearBodyList();
			//		unitGround->Kill();

			//		for (int i = 0; i < 2; i++) {
			//			unitSlugs[i]->Kill();
			//		}

			//		//Level generation
			//		level = new cLevel(hge, physic);
			//		level->Create (1024*2, iScreenHeight, deform_test, ground_test, sky_test);	
			//		level->generateMap();
			//		//level->textureLevel(tex_test);

			//		//Change Scene
			//		iScene = SCENE_LEVEL_TEST;
			//		sfx->PlaySnd(false);
			//	}
			//	if (multiplayer->Collision(fMouseX, fMouseY)) {
			//		iScene = SCENE_MENUE;
			//		sfx->PlaySnd(false);
			//	}
			//	if (options->Collision(fMouseX, fMouseY)) {
			//		iScene = SCENE_MENUE;	
			//		sfx->PlaySnd(false);
			//	}
			//}
			break;
		case SCENE_GAME:
			
			break;
		case SCENE_MENUE_SINGLEPLAYER:
			break;
		case SCENE_MENUE_MULTYPLAYER:
			break;
		case SCENE_MENUE_OPTIONS:
			break;
		case SCENE_OPTIONS_SOUND:
			break;
		case SCENE_OPTIONS_GRAPHIC:
			break;
		case SCENE_OPTIONS_TEAM:
			break;
		case SCENE_SOUND_TEST:
			/*if (hge->Input_GetKeyState(HGEK_UP)) if (sfx->get_volume() < 100) sfx->change_volume(+1);
			if (hge->Input_GetKeyState(HGEK_DOWN)) if (sfx->get_volume() > 0) sfx->change_volume(-1);
			if (hge->Input_GetKeyState(HGEK_LEFT)) if (sfx->get_pan() > -100) sfx->change_pan(-1);
			if (hge->Input_GetKeyState(HGEK_RIGHT)) if (sfx->get_pan() < 100) sfx->change_pan(+1);
			if (hge->Input_GetKeyState(HGEK_SPACE)) sfx->Stop();
			if (hge->Input_GetKeyState(HGEK_ENTER)) sfx->PlaySnd(true);*/
			break;
		case SCENE_PHYSIC_TEST:

			break;
		case SCENE_LEVEL_TEST:
				
			Object* selectedObject = World::Get()->SelectedObject();

			if (myEvent->Type == sf::Event::KeyPressed)
			{

				if (myEvent->Key.Code == sf::Key::G)
					World::Get()->Regenerate();
				else if (myEvent->Key.Code == sf::Key::S)
				{

					char str[32];
					sprintf_s(str, 32, "Seed: %u", World::Get()->Seed());
					MessageBox(NULL, str, "Information", MB_OK);

				}
				else if (myEvent->Key.Code == sf::Key::L)
				{

					lockCameraToLevel = !lockCameraToLevel;
				
				}
				else if (selectedObject)
				{
					
					if (myEvent->Key.Code == sf::Key::Left)
						selectedObject->StartMovingLeft();
					else if (myEvent->Key.Code == sf::Key::Right)
						selectedObject->StartMovingRight();
					else if (myEvent->Key.Code == sf::Key::Up)
						selectedObject->StartAimingUp();
					else if (myEvent->Key.Code == sf::Key::Down)
						selectedObject->StartAimingDown();
					else if (myEvent->Key.Code == sf::Key::Return)
						selectedObject->Jump();
					else if (myEvent->Key.Code == sf::Key::Space)
						selectedObject->StartCharging();

				}

			}
			else if (myEvent->Type == sf::Event::KeyReleased)
			{

				if (selectedObject)
				{

					if (myEvent->Key.Code == sf::Key::Left)
						selectedObject->StopMoving();
					else if (myEvent->Key.Code == sf::Key::Right)
						selectedObject->StopMoving();
					else if (myEvent->Key.Code == sf::Key::Up)
						selectedObject->StopAiming();
					else if (myEvent->Key.Code == sf::Key::Down)
						selectedObject->StopAiming();
					else if (myEvent->Key.Code == sf::Key::Space)
						selectedObject->Fire();

				}
			
			}
			else if (myEvent->Type == sf::Event::MouseButtonPressed)
			{

				if (myEvent->MouseButton.Button == sf::Mouse::Left)
				{
					
					World::Get()->SelectObjectAtPosition(World::Get()->ToWorldCoordinates(cam, iScreenWidth, iScreenHeight, myEvent->MouseButton.X, myEvent->MouseButton.Y));

				}
				else if (myEvent->MouseButton.Button == sf::Mouse::Middle)
				{

					Slug* slug = new Slug();
					ImageResource* r = (ImageResource*)ResourceManager::Get()->GetResource("image_gravestone");
					slug->SetImage(r);
					slug->SetPosition(World::Get()->ToWorldCoordinates(cam, iScreenWidth, iScreenHeight, myEvent->MouseButton.X, myEvent->MouseButton.Y));
					slug->SetVelocity(0, 200);
					slug->SetRadius(8);
					World::Get()->AddObject(slug);

				}
				else
					rmbDown = true;

			}
			else if (myEvent->Type == sf::Event::MouseButtonReleased)
			{

				if (myEvent->MouseButton.Button == sf::Mouse::Right)
					rmbDown = false;

			}
			else if (myEvent->Type == sf::Event::MouseMoved)
			{

				// RMB Camera Drag
				if (rmbDown)
				{

					int mx = iMouseX - lastMouseX;
					int my = iMouseY - lastMouseY;

					sf::Rect<float> viewRect = cam->GetRect();

					if (lockCameraToLevel)
					{

						if (mx < 0)
						{

							if (-mx > (int)viewRect.Left)
								mx = -(int)viewRect.Left; 

						}
						else
						{

							int d = World::Get()->WidthInPixels() - (int)viewRect.Right;
							
							if (d < mx)
								mx = d;

						}

						if (my < 0)
						{

							int d = -World::Get()->HeightInPixels() - (int)viewRect.Top;

							if (d > my)
								my = d;

						}
						else
						{

							if (my > -(int)viewRect.Bottom)
								my = -(int)viewRect.Bottom;

						}

					}

					cam->Move((float)mx, (float)my);

					World::Get()->CameraMoved(cam->GetCenter());

				}

			}

			break;
	}
	
	return true;
}

void cSlugs::bRender() {

	
	Renderer::Get()->Clear(Color(100, 100, 100));

	switch (iScene) {
		case SCENE_LOGO:
			break;
		case SCENE_MENUE:
			Menu();
			break;		
		case SCENE_XML_TEST:
			XMLTest();
			break;
		case SCENE_ENGINE_TEST:
			GraphicTest();
			break;
		case SCENE_SOUND_TEST:
			SoundTest();
			break;
		case SCENE_LEVEL_TEST:
			LevelTest();
			break;
		case SCENE_PHYSIC_TEST:
			PhysicTest();
			break;
		case SCENE_MENUE_OPTIONS:
			break;
		case SCENE_MENUE_SINGLEPLAYER:
			break;
		case SCENE_MENUE_MULTYPLAYER:
			break;
		case SCENE_OPTIONS_SOUND:
			break;
		case SCENE_OPTIONS_GRAPHIC:
			break;
		case SCENE_OPTIONS_TEAM:
			break;
		case SCENE_GAME:
			Game();
			break;
	}

	//FPS
	sprintf_s(sFPS, "FPS: %.1f", fps);
	font.Draw(5, 5, sFPS);	

	Renderer::Get()->Present();

}

bool cSlugs::bInit()
{

	//Loading XML
	if (!config.bLoad(global.CombiStr("data\\config.xml"))) return false;

	iScreenWidth = atoi(config.get_element("screen_width")->GetText());
	iScreenHeight = atoi(config.get_element("screen_height")->GetText());

	//Init Engine
	Renderer::Get()->Initialize(iScreenWidth, iScreenHeight, "Slugs");

	cam = new sf::View(sf::FloatRect(0, 0, (float)iScreenWidth, (float)iScreenHeight));
	Renderer::Get()->SetView(*cam);

	//Logo
	sf::Image img_icon;
	if (!img_icon.LoadFromFile(global.CombiStr("Slug.png"))) 
		return false;

	Renderer::Get()->SetIcon(img_icon); 

	if (strcmp(config.get_element("fullscreen")->GetText(), "true") == 0) {
		bFullscreen = false;
	} else {
		bFullscreen = true;
	}

	// Initialize resource manager and world, load test textures
	ResourceManager* resourceManager = ResourceManager::Get();

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
	resourceManager->AddResource("image_slug_left", new ImageResource("gfx\\ColoredSlugSmallLeft.png"));

	resourceManager->AddResource("image_snowflake", new ImageResource("gfx\\snowflake.tga"));

	resourceManager->AddResource("menu_click", new SoundResource("sfx\\menu_click.WAV"));

	TextureBuffer* rawTex[3];
	rawTex[0] = new TextureBuffer("gfx\\levels\\test\\ground.raw", 256, 256, 4);
	resourceManager->AddResource("test_ground", rawTex[0]);
	rawTex[1] = new TextureBuffer("gfx\\levels\\test\\over.raw", 256, 12, 4);
	resourceManager->AddResource("test_over", rawTex[1]);
	rawTex[2] = new TextureBuffer("gfx\\levels\\test\\under.raw", 256, 4, 4);
	resourceManager->AddResource("test_under", rawTex[2]);

	//---------------------------------------------fonts-------------------------

	if (!font.bLoad(&Renderer::Get()->GetWindow(), global.CombiStr("gfx\\fonts\\arial.ttf"))) return false; 
	if (!font2.bLoad(&Renderer::Get()->GetWindow(), global.CombiStr("gfx\\fonts\\arial.ttf"))) return false;
	if (!font3.bLoad(&Renderer::Get()->GetWindow(), global.CombiStr("gfx\\fonts\\arial.ttf"))) return false;

	//----------------------------Sounds--------------------------------------------

	if (!sfx.bLoadSnd((SoundResource*)resourceManager->GetResource("menu_click"))) return false;

	//only if its the right scene (later we have to do it on some click event)
	if (iScene == SCENE_LEVEL_TEST)
		InitLevelTest();
	
	return true;
}

void cSlugs::InitLevelTest()
{

	World* world = World::Get();
	ResourceManager* resources = ResourceManager::Get();

	world->Build(2, 2, (TextureBuffer*)resources->GetResource("tb_ground"), (TextureBuffer*)resources->GetResource("tb_over"), (TextureBuffer*)resources->GetResource("tb_under"), (ImageResource*)resources->GetResource("image_water0"));
	//world->SetWaterColor(sf::Color(255, 0, 0));
	world->SetBackground((ImageResource*)resources->GetResource("image_backgroundfar"), (ImageResource*)resources->GetResource("image_backgroundnear"));

	cam->SetCenter((float)iScreenWidth / 2.0f, -(float)iScreenHeight);
	lockCameraToLevel = true;

	world->CameraMoved(cam->GetCenter());

	FXManager* fxManager = FXManager::Get();
	fxManager->ClearEffects();
	
	SnowSystem* snowSystem = new SnowSystem((ImageResource*)resources->GetResource("image_snowflake"), 1.0f);
	fxManager->RegisterEffect(snowSystem);

}

void cSlugs::Shutdown()
{

	World::Get()->Destroy();
	FXManager::Get()->Destroy();
	ResourceManager::Get()->Destroy();

}

void cSlugs::Clear() {
		 
}

void cSlugs::Game() {
	
}

void cSlugs::Menu() {

	sprintf_s(buffer, "ver. %1.3f", VERSION);
	font2.setColor(sf::Color(0, 0, 0));
	font2.Draw((float)iScreenWidth-65, (float)iScreenHeight-20, buffer);

}

void cSlugs::GraphicTest()
{

}

void cSlugs::SoundTest()
{

}

void cSlugs::PhysicTest() {
	
}

void cSlugs::XMLTest() {
}

void cSlugs::LevelTest()
{

	World* world = World::Get();

	if (world)
		world->Render();

	if (world->SelectedObject())
	{

		Vector2 pos = world->SelectedObject()->Position();
		char str[64];
		sprintf_s(str, 64, "%i", world->SelectedObject()->HitPoints());
		font.Draw(pos.x, pos.y - 30, str);

	}
	
}