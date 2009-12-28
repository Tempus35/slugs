#pragma once

#include <sfml/system.hpp>

#include "debug.h"
#include "converters.h"
#include "singleton.h"
#include "world.h"
#include "fxmanager.h"
#include "updatemanager.h"
#include "player.h"
#include "camera.h"
#include "resourcemanager.h"

/*
	Enumeration of bool game flags
*/

enum GameBool
{

	GameBool_CanChooseSlug,						// Allow players to choose which slug they want to use each turn
	GameBool_CanUseMultipleSlugsPerTurn,		// Allow switching slugs during the turn?
	GameBool_AlwaysShowNames,					// Always show slug names?
	GameBool_AlwaysShowHps,						// Always show slug hitpoints?
	GameBool_Debug,								// Show debugging info
	GameBool_LockCameraToLevel,					// Lock the camera so that it stays inside the level bounds
	GameBool_Pause,								// Is the game paused?

	GameBool_LAST,								// MUST BE LAST ON THE LIST

};

/*
	Enumeration of floating point game flags
*/

enum GameFloat
{

	GameFloat_TurnTime,							// Time per turn in seconds
	GameFloat_TurnEndTime,						// Delay in seconds at the end of a turn before the next one begins

	GameFloat_LAST,								// MUST BE LAST ON THE LIST
};

/*
	Enumeration of integer game flags
*/

enum GameInt
{

	GameInt_SlugsPerTeam,						// Numbers of slugs on each team
	GameInt_ShotsPerTurn,						// Number of shots that can be fired before the turn ends

	GameInt_LAST,								// MUST BE LAST ON THE LIST

};

/*
	Enumeration of game states. These represent states that the game can be in, such as on a menu, or in the game world
*/

enum GameState
{

	GameState_None,					// Initial state when the app is loaded
	GameState_MainMenu,				// On the main menu
	GameState_Game,					// In the game world

};

class Game : public Singleton<Game>
{

friend class Singleton<Game>;

private:

	std::vector<bool>				gameBools;			// List of game flags
	std::vector<float>				gameFloats;
	std::vector<int>				gameInts;

	World*							world;				// Pointer to the game world

	UpdateManager*					updateManager;		// Pointer to UpdateManager instance
	FXManager*						fxManager;			// Pointer to FXManager instance

	std::vector<Player*>			players;			// List of all players currently in the game
	unsigned int					activePlayer;		// Index of the currently active player

	Camera*							camera;				// Camera instance

	GameState						state;				// Current game state

	bool							noAction;			// Set to true if nothing moved on the last update

	Vec2i							cursorPosition;		// Current position of the mouse cursor in screen space

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

	// Gets the current value of a game flag
	bool GetGameBool(GameBool flag) const;
	float GetGameFloat(GameFloat flag) const;
	int GetGameInt(GameInt flag) const;

	// Sets the current value of a game flag
	void SetGameBool(GameBool flag, bool value);
	void ToggleGameBool(GameBool flag);
	void SetGameFloat(GameFloat flag, float value);
	void SetGameInt(GameInt flag, int value);

	// Gets the default value for a game flag
	bool GetGameDefaultBool(GameBool flag) const;
	float GetGameDefaultFloat(GameFloat flag) const;
	int GetGameDefaultInt(GameInt flag) const;

	// Ends the turn of the current player and moves to the next player
	void EndTurn();

	// Gets the currently active player 
	Player* GetCurrentPlayer() const;

	// Returns true if anything is currently going on in the game
	bool EnsureNoAction() const;

};