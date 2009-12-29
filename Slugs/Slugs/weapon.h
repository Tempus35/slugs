//---------------------------------------------------------------
//
// Slugs
// weapon.h
//
//---------------------------------------------------------------

#pragma once

#include "global.h"

#include "trajectory.h"
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
	WeaponType_Mine,
	WeaponType_HomingMissile,
	WeaponType_Teleporter,
	WeaponType_Airstrike,
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
	bool			requiresAiming;		// True if this weapon requires aiming

protected:

	// Constructor
	Weapon(WeaponType t, int initialAmmo, bool charges, bool aims);

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

	// Fires the weapon, returns true if the weapon fired. projectileCreated should be set to the launched projectile (if any) for camera tracking
	virtual bool Fire(Slug* owner, Projectile*& projectileCreated) = 0;

	// Returns true if the weapon is targetable (default implementation returns false)
	virtual bool IsTargetable() const;

	// Gets the launch speed for a given power level (should be overriden, default implementation returns 0.0f)
	virtual float GetLaunchSpeed(float power = 1.0f) const;

	// Renders debugging information
	virtual void DebugRender();

	// Returns true if this weapon requires aiming
	virtual bool RequiresAiming() const;

};

/*
	class TargetableWeapon
	A weapon that requires the user to set a target with the mouse
*/

class TargetableWeapon : public Weapon
{

protected:

	bool			targetSelected;		// True if a target point has been selected
	Vec2f			targetPoint;		// Current target point for the weapon

public:

	// Constructor
	TargetableWeapon(WeaponType t, int initialAmmo, bool charges, bool aims);

	// Sets the target point for the weapon
	virtual void SetTargetPoint(const Vec2f& point);

	// Returns true
	virtual bool IsTargetable() const;

	// Returns true if the weapon has a target set
	virtual bool HasTarget() const;

	// Returns the targeted point
	virtual const Vec2f& GetTarget() const;


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
	
	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	virtual float GetLaunchSpeed(float power = 1.0f) const;

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

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	virtual float GetLaunchSpeed(float power = 1.0f) const;

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

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

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

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	// Hooked so that we can fire the machinegun over time
	virtual void Update(float elapsedTime);


};

/*
	class Weapon_Mine
	Drops mines
*/

class Weapon_Mine : public Weapon
{

protected:

public:

	Weapon_Mine(int initialAmmo = -1);

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

};

/*
	class Weapon_HomingMissile
*/

class Weapon_HomingMissile : public TargetableWeapon
{

protected:

public:

	Weapon_HomingMissile(int initialAmmo = -1);

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	virtual float GetLaunchSpeed(float power = 1.0f) const;

};

/*
	class Weapon_Teleporter
*/

class Weapon_Teleporter : public TargetableWeapon
{

protected:

public:

	Weapon_Teleporter(int initialAmmo = -1);

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	// From TargetableWeapon - overriden to prevent teleporting into terrain/objects
	virtual void SetTargetPoint(const Vec2f& point);

};

/*
	class Weapon_Airstrike
*/

class Weapon_Airstrike : public TargetableWeapon
{

protected:

	Vec2f launchDirection;
	Vec2f launchPoint;

public:

	Weapon_Airstrike(int initialAmmo = -1);

	virtual bool Fire(Slug* owner, Projectile*& projectileCreated);

	virtual float GetLaunchSpeed(float power = 1.0f) const;

	virtual void DebugRender();

};