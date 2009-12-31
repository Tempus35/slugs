#pragma once

#include <direct.h>
#include <sfml/graphics.hpp>
#include <sfml/audio.hpp>
#include <sfml/config.hpp>
#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/network.hpp>

#include "global.h"
#include "renderer.h"
#include "resourcemanager.h"
#include "game.h"
#include "ini.h"
#include "localizer.h"
#include "log.h"

class App
{

private:

	std::string			workingDirectory;		// Application path at startup

	unsigned int		frame;					// Current frame number
	float				fps;					// Current frames per second
	float				time;					// Total elapsed time in seconds since app started
	bool				showFps;				// Show the fps counter?

	bool				exit;					// Exit flag used to stop the application

	Vec2i				currentMousePos;		// Current mouse coordinates in screen space
	Vec2i				previousMousePos;		// Previous mouse coordinates in screen space
	bool				mouseButtonStates[3];   // Current mouse button state (left, right, middle)

	char				fpsBuffer[32];			// Buffer used by the fps counter when rendering

	sf::Clock			clock;					// Timer used to get elapsed time between frames

public:

	App();
	~App();

	// Updates the FPS calculation
	void UpdateFPS(float elapsedTime);

	// Updates everything
	void MainLoop();

	// Renders everyting
	void Render();

	// Closes the application
	void Exit();

	// Main event handler
	bool HandleEvent(sf::Event* e);

	// Handles key presses
	bool HandleKeyDown(sf::Key::Code key, bool shift, bool control, bool alt);

	// Handles key releases
	bool HandleKeyUp(sf::Key::Code key, bool shift, bool control, bool alt);

	// Handles mouse button presses
	bool HandleMouseDown(sf::Mouse::Button button);

	// Handles mouse button releases
	bool HandleMouseUp(sf::Mouse::Button button);

	// Handles mouse movement
	bool HandleMouseMoved();

};