#pragma once

#include <vector>
#include <string>

#include "debug.h"
#include "resourcemanager.h"
#include "slug.h"
#include "weaponstore.h"
#include "converters.h"

/*
	Forward declarations
*/

class Player;

/*
	class Team
	Tracks a team of slugs
*/

class Team
{

private:

	std::string				name;				// Team Name
	Color					color;				// Team color;

	Player*					owner;				// The player to which this team belongs

	std::vector<Slug*>		slugs;				// Slugs on the team

	WeaponStore*			weaponStore;		// Pointer to the teams weapon store if using team based weapons

public:

	// Initialization
	Team(Player* player);
	~Team();

	// Gets the name of the team
	const std::string& GetName() const;

	// Sets the name of the team
	void SetName(const std::string& newName);

	// Gets the color of the team
	const Color& GetColor() const;

	// Sets the color of the team
	void SetColor(const Color& newColor);

	// Gets the owning player
	Player* GetPlayer() const;

	// Does this team contain the slug?
	bool Contains(Slug* slug) const;

	// Adds a slug to the team, this function takes ownership of the Slug instance
	bool Add(Slug* slug);

	// Set the weaponstore for the team, takes ownership of the instance
	void SetWeapons(WeaponStore* store);

	// Randomizes the team, it will still need a weapon store
	void Randomize(int numSlugs = 5);

	// Gets slugs on the team
	void GetSlugs(std::vector<Slug*>& list, bool aliveOnly = true);

	// Places all slugs on the team into the world
	void PlaceInWorld();

	// Gets the total remaining health of the team
	int GetTotalHitpoints() const;

	// Returns true if the team has any slugs left alive
	bool HasAliveSlugs() const;

};