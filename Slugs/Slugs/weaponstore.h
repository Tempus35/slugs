//---------------------------------------------------------------
//
// Slugs
// weaponstore.h
//
//---------------------------------------------------------------

#pragma once

#include <vector>

#include "debug.h"
#include "utility.h"

#include "weapon.h"

/*
	class WeaponStore
	Storage for a group of weapons. Can be assigned to teams oor individual slugs
*/

class WeaponStore
{

private:

	std::vector<Weapon*> weapons;

public:

	// Initialization
	WeaponStore(bool debug = false);
	~WeaponStore();

	// Adds a weapon to the store
	// This function takes ownership of the weapon instance
	void Add(Weapon* weapon);

	// Gets the first weapon that has ammo from the store
	Weapon* Get() const;

	// Gets a weapon from the store
	Weapon* Get(WeaponType type, bool getEmptyWeapons = true) const;

};