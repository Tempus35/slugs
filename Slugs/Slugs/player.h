#pragma once

#include "debug.h"
#include "team.h"

/*
	class Player
	Representation of a single player
*/

class Player
{

protected:

	std::vector<Team*>		teams;			// List of teams that the player has in play
	Team*					activeTeam;		// Currently active team
	Slug*					activeSlug;		// Currently active slug

public:

	// Constructor
	Player();

	// Fired when the players turn begins
	virtual void TurnBegins();

	// Adds a team tot he players team list
	void AddTeam(Team* team);

	// Places all the players teams in the world
	void PlaceInWorld();

	// Makes the next slug in the chain active
	void SelectNextSlug();

	// Makes the camera move to the position of the active slug
	void MoveCameraToActiveSlug();

};