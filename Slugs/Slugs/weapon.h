//---------------------------------------------------------------
//
// Slugs
// weapon.h
//
//---------------------------------------------------------------

#pragma once

#include "debug.h"

#include "projectile.h"
#include "updatemanager.h"

/*
	Forward declarations
*/

class Slug;

/*
	Enumeration of all available weapon types
*/

enum WeaponType
{

	WeaponType_Bazooka,
	WeaponType_Grenade,
	WeaponType_Shotgun,
	WeaponType_Machinegun,
	WeaponType_LAST,					// Should be last on the list, used to determine number of available weapon types

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

	// Creates a weapon from the given type
	static Weapon* CreateFromType(WeaponType t);

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
	virtual bool Fire(Slug* owner) = 0;

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
	
	virtual bool Fire(Slug* owner);

};

/*
	class Weapon_Grenade
	Standard grenade
*/

class Weapon_Grenade : public Weapon
{

protected:

public:

	Weapon_Grenade(int initialAmmo = -1);

	virtual bool Fire(Slug* owner);

};

/*
	class Weapon_Shotgun
	Shotgun weapon - fires instantly in a spread pattern
*/

class Weapon_Shotgun : public Weapon
{

protected:

public:

	Weapon_Shotgun(int initialAmmo = -1);

	virtual bool Fire(Slug* owner);

};

/*
	class Weapon_Machinegun
	Machine gun weapon - fires a spray of bullets
*/

class Weapon_Machinegun : public Weapon, public Updateable
{

protected:

	bool		firing;					// Is the machinegun currently firing
	float		fireTimer;				// Shot timer
	int			fireCounter;			// Number of shots remaining before we stop firing
	Slug*		slug;					// The slug unlucky enough to fire this weapon last

public:

	Weapon_Machinegun(int initialAmmo = -1);

	virtual bool Fire(Slug* owner);

	// Hooked so that we can fire the machinegun over time
	virtual void Update(float elapsedTime);


};