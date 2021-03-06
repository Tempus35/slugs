#pragma once

#include "debug.h"
#include "team.h"

/*
	Enumeration of player types
*/

enum PlayerType
{

	PlayerType_Local,					// Player by the local user
	PlayerType_Computer,				// Played the local computer
	PlayerType_Network,					// Played by another user (or their computer) on the network

};

/*
	class Player
	A player in the game played by the local user
*/

class Player
{

protected:

	std::string				name;			// Name of the player
	PlayerType				type;			// Type of the player (local, computer, network)

	std::vector<Team*>		teams;			// List of teams that the player has in play
	Team*					activeTeam;		// Currently active team
	Slug*					activeSlug;		// Currently active slug

	float					timeRemaining;	// Time remaining this turn in seconds
	int						shotsRemaining;	// Numbers of shots still available to the player this turn
	bool					acted;			// Has the use already acted with a slug this turn?

	bool					turnEnding;		// Is the turn in the proces of ending
	float					turnEndTimer;	// Delay timer used to end the turn

public:

	// Constructor
	Player(const std::string& playerName, PlayerType playerType = PlayerType_Local);

	// Gets the name of the player
	const std::string& GetName() const;

	// Gets the type of the player
	PlayerType GetType() const;

	// Gets the currently active slug of the player
	Slug* GetCurrentSlug() const;

	// Updates player related variables
	void Update(float elapsedTime);

	// Fired when the players turn begins
	virtual void TurnBegins();

	// Resets all turn variables for the start of a turn
	virtual void ResetForTurn();

	// Fired when the players turn ends
	virtual void TurnEnds();

	// Fired when a slug uses a weapon
	virtual void SlugFired();

	// Adds a team tot he players team list
	virtual void AddTeam(Team* team);

	// Places all the players teams in the world
	virtual void PlaceInWorld();

	// Makes the next slug in the chain active
	virtual void SelectNextSlug();

	// Makes the camera move to the position of the active slug
	virtual void MoveCameraToActiveSlug();

	// Gets the remaining turn time for the player
	virtual float GetTurnTimeRemaining() const;

	// Returns true if the player has performed any action other than choosing a slug this turn
	virtual bool HasActed() const;

	// Sets the acted flag for the player. This is reset at the start of the next turn.
	virtual void Acted();

	// Returns true if the current turn is in the process of ending
	virtual bool IsTurnEnding() const;

	// Sets the turn ending flag
	virtual void EndTurn();

	// Renders debugging information
	virtual void DebugRender();

	// Gets the number of teams the player owns
	virtual unsigned int GetNumTeams() const;

	// Gets a team
	virtual Team* GetTeam(int index) const;

	// Returns true if the player has any slugs still alive
	virtual bool HasAliveSlugs() const;

};