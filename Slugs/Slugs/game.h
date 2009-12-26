#pragma once

#include <sfml/system.hpp>

#include "debug.h"
#include "singleton.h"
#include "world.h"
#include "fxmanager.h"
#include "updatemanager.h"
#include "player.h"
#include "camera.h"
#include "resourcemanager.h"

/*
	Enumeration of game flags
	Each flag is a boolean
*/

enum GameFlag
{

	GameFlag_CanChooseSlug,						// Allow players to choose which slug they want to use each turn
	GameFlag_AlwaysShowNames,					// Always show slug names?
	GameFlag_LAST,								// Must be last on the list

};

/*
	Enumeration of game states
*/

enum GameState
{

	GameState_None,					// Initial state when the app is loaded
	GameState_Game,					// In the game world

};

class Game : public Singleton<Game>
{

friend class Singleton<Game>;

private:

	std::vector<bool>				flags;				// List of game flags

	World*							world;				// Pointer to the game world

	UpdateManager*					updateManager;		// Pointer to UpdateManager instance
	FXManager*						fxManager;			// Pointer to FXManager instance

	std::list<Player*>				players;			// List of all players currently in the game

	Camera*							camera;				// Camera instance
	bool							lockCameraToLevel;  // Is the camera locked to the bounds of the level?

	GameState						state;				// Current game state

private:

	Game();
	~Game();

public:

	// Update the game
	void Update(float elapsedTime);

	// Render the game
	void Render();

	// Gets a pointer to the world
	World* GetWorld() const;

	// Gets a pointer to the update manager
	UpdateManager* GetUpdateManager() const;

	// Gets a pointer to the fx manager
	FXManager* GetFXManager() const;

	// Returns true if the specified flag is set
	bool IsFlagSet(GameFlag flag) const;

	// Handles key presses
	bool KeyDown(sf::Key::Code key, bool shift, bool control, bool alt);

	// Handles key releases
	bool KeyUp(sf::Key::Code key, bool shift, bool control, bool alt);

	// Handles mouse button presses
	bool MouseDown(const Vec2i& position, sf::Mouse::Button button);

	// Handles mouse button releases
	bool MouseUp(const Vec2i& position, sf::Mouse::Button button);

	// Handles mouse movement
	bool MouseMoved(bool left, bool right, bool middle, const Vec2i& from, const Vec2i& to);

	// Loads resources for a given game state
	void LoadResourcesForState(GameState gameState);

	// Changes the game state
	void ChangeGameState(GameState gameState);

	// Creates and sets up the world
	void CreateWorld();
		
	// Gets the game camera
	Camera* GetCamera() const;

};