#pragma once

#include "debug.h"

/*
	Enumeration of available weapon types
*/

enum WeaponType
{

	WeaponType_Bazooka,

};

/*
	class Weapon
	Base class for weapons
*/

class Weapon
{

protected:

	WeaponType		type;				// Type identifier
	int				ammo;				// Ammo remaining (-1 is infinite ammo)
	bool			needsCharge;		// True if this weapon charges, false if it fires instantly

protected:

	// Constructor
	Weapon(WeaponType t, int initialAmmo, bool charges);

public:

	// Returns the type of the weapon
	WeaponType GetType() const;

	// Returns the amount of ammo left for the weapon
	virtual int GetAmmo() const;
	
	// Returns true if the weapon requires charging
	virtual bool NeedsCharge() const;

	// Reduces remaining ammo, returns true if weapon can fire
	virtual bool TakeAmmo();

	// Fires the weapon, returns true if the weapon fired
	virtual bool Fire() = 0;

};

/*
	class Weapon_Bazooka
	Standard bazooka
*/

class Weapon_Bazooka : public Weapon
{

protected:

public:

	Weapon_Bazooka(int initialAmmo = -1);
	
	virtual bool Fire();

};