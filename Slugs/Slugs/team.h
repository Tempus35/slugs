#pragma once

#include <vector>
#include <string>

#include "debug.h"
#include "slug.h"
#include "weaponstore.h"

using namespace std;

/*
	class Team
	Tracks a team of slugs
*/

class Team
{

private:

	std::string				name;				// Team Name

	vector<Slug*>			slugs;				// Slugs on the team

	int						numAlive;			// Number of alive slugs remaining
	int						activeIndex;		// Index of currently active slug
	int						maxHealth;			// Total initial health of the team
	int						currentHealth;		// Current health of the team

	WeaponStore*			weaponStore;		// Pointer to the teams weapon store if using team based weapons

public:

	// Initialization
	Team();
	~Team();

	// Team name accessors
	const std::string& GetName() const;
	void SetName(const std::string& newName);

	// Does this team contain the slug?
	bool Contains(Slug* slug) const;

	// Adds a slug to the team
	// This function takes ownership of the Slug instance
	bool Add(Slug* slug);

	// Selection
	Slug* Next();
	Slug* Previous();
	Slug* First();
	Slug* Last();

	// Weapons
	// This function takes ownership of the WeaponStore instance
	void SetWeapons(WeaponStore* store);

};