#pragma once

#include <vector>
#include <string>

#include "debug.h"
#include "resourcemanager.h"
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
	Color					color;				// Team color;

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

	// Gets the name of the team
	const std::string& GetName() const;

	// Sets the name of the team
	void SetName(const std::string& newName);

	// Gets the color of the team
	const Color& GetColor() const;

	// Sets the color of the team
	void SetColor(const Color& newColor);

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

	// Set the weaponstore for the team, takes ownership of the instance
	void SetWeapons(WeaponStore* store);

	// Randomizes the team, it will still need a weapon store
	void Randomize(int numSlugs = 5);

	// Gets slugs on the team
	void GetSlugs(std::vector<Slug*>& list, bool aliveOnly = true);

	// Places all slugs on the team into the world
	void PlaceInWorld();

};