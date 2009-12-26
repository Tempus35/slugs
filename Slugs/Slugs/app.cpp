#include "app.h"

App::App()
{

	//
	// Get and store current working directory
	//

	char* dir = _getcwd(NULL, 0);
	workingDirectory = dir;
	free(dir);

	//
	// Load configuration settings
	//

	IniReader ini(workingDirectory + "\\slugs.ini");

	Vec2i resolution;
	resolution.x = ini.ReadInt("display", "width", 1024);
	resolution.y = ini.ReadInt("display", "height", 768);

	bool fullscreen = ini.ReadBool("display", "fullscreen", false);

	//
	// Setup states
	//

	frame = 0;
	fps = 0;
	time = 0.0f;
	exit = false;

	clock.Reset();

	mouseButtonStates[0] = false;
	mouseButtonStates[1] = false;
	mouseButtonStates[2] = false;

	//
	// Set application icon
	//

	sf::Image icon;
	std::string iconPath = workingDirectory + "\\Slug.png";
	icon.LoadFromFile(iconPath);
	Renderer::Get()->SetIcon(icon); 

	//
	// Initialize renderer and game
	//

	Renderer::Get()->Initialize(resolution.x, resolution.y, "Slugs", !fullscreen);
	Game::Get()->GetCamera()->SetViewSize(resolution.x, resolution.y);

}

App::~App()
{

	//
	// Write configuration settings
	//

	IniWriter ini(workingDirectory + "\\slugs.ini");

	ini.WriteInt("display", "width", Renderer::Get()->GetWidth());
	ini.WriteInt("display", "height", Renderer::Get()->GetHeight());
	ini.WriteBool("display", "fullscreen", Renderer::Get()->IsFullscreen());

	//
	// Destroy singletons
	//

	Renderer::Get()->Destroy();
	Game::Get()->Destroy();
	ResourceManager::Get()->Destroy();

}

void App::UpdateFPS(float elapsedTime)
{

	frame++;
	time += elapsedTime;

	if (time > 1e-4f)
		fps = (float)frame / time;
	else
		fps = 0;

}

void App::MainLoop()
{

	Game::Get()->ChangeGameState(GameState_Game);

	while (!exit)
	{

		// Get time since last frame
		float elapsedTime = clock.GetElapsedTime();
		clock.Reset();

		// Update FPS counter
		UpdateFPS(elapsedTime);

		// Process SFML events
		sf::Event Event;
		while (Renderer::Get()->GetWindow().GetEvent(Event))
		{
			
			if (!HandleEvent(&Event)) 
				exit = true;

		}

		// Update the game
		Game::Get()->Update(Min(elapsedTime, 0.1f));

		// Render
		Render();

	}

}

bool App::HandleKeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	switch (key)
	{

	case sf::Key::Escape:
		return false;

	case sf::Key::Return:

		if (alt == true)
		{

			Renderer::Get()->SetFullscreen(!Renderer::Get()->IsFullscreen());		
			return true;

		}

		break;

	}

	// Pass to game for further processing
	return Game::Get()->KeyDown(key, shift, control, alt);

}

bool App::HandleKeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	// Pass to game for further processing
	return Game::Get()->KeyUp(key, shift, control, alt);

}

bool App::HandleMouseDown(sf::Mouse::Button button)
{

	switch (button)
	{

	case sf::Mouse::Left:
		mouseButtonStates[0] = true;
		break;

	case sf::Mouse::Right:
		mouseButtonStates[1] = true;
		break;

	case sf::Mouse::Middle:
		mouseButtonStates[2] = true;
		break;

	default:
		return true;

	}

	// Pass to game for further processing
	return Game::Get()->MouseDown(currentMousePos, button);

}

bool App::HandleMouseUp(sf::Mouse::Button button)
{

	switch (button)
	{

	case sf::Mouse::Left:
		mouseButtonStates[0] = false;
		break;

	case sf::Mouse::Right:
		mouseButtonStates[1] = false;
		break;

	case sf::Mouse::Middle:
		mouseButtonStates[2] = false;
		break;

	default:
		return true;

	}

	// Pass to game for further processing
	return Game::Get()->MouseUp(currentMousePos, button);

}

bool App::HandleMouseMoved()
{

	// Pass to game for further processing
	return Game::Get()->MouseMoved(mouseButtonStates[0], mouseButtonStates[1], mouseButtonStates[2], previousMousePos, currentMousePos);

}

bool App::HandleEvent(sf::Event* e)
{


	switch (e->Type)
	{

	case sf::Event::Closed:
		return false;

	case sf::Event::KeyPressed:

		return HandleKeyDown(e->Key.Code, e->Key.Shift, e->Key.Control, e->Key.Alt);

		break;

	case sf::Event::KeyReleased:

		return HandleKeyUp(e->Key.Code, e->Key.Shift, e->Key.Control, e->Key.Alt);

		break;

	case sf::Event::MouseButtonPressed:

		return HandleMouseDown(e->MouseButton.Button);

		break;

	case sf::Event::MouseButtonReleased:

		return HandleMouseUp(e->MouseButton.Button);

		break;

	case sf::Event::MouseMoved:

		previousMousePos = currentMousePos;
		currentMousePos = Vec2i(e->MouseMove.X, e->MouseMove.Y);

		return HandleMouseMoved();

		break;

	}

	return true;

}

void App::Render()
{

	// Clear backbuffer
	Renderer::Get()->Clear(Color(100, 100, 100));

	// Render game
	Game::Get()->Render();

	Renderer::Get()->SetCamera(NULL);

	// Draw FPS counter
	sprintf_s(fpsBuffer, "FPS: %.1f", fps);
	Renderer::Get()->RenderTextShadowed(10, 10, (FontResource*)ResourceManager::Get()->GetResource("font_arial"), fpsBuffer);

	// Present frame
	Renderer::Get()->Present();

}

void App::Exit()
{

	exit = true;

}