#pragma once

#include "object.h"
#include "projectile.h"
#include "weapon.h"

/*
	class Pickup
	Base class for all objects that can be collected by slugs
*/

class Pickup : public Object
{

protected:

public:

	// Constructor
	Pickup();

	// Overriden for pickup processing
	virtual void OnCollideWithObject(Object* object);

};

/*
	class Pickup_Bomb
	Explodes when picked up. LOL.
*/

class Pickup_Exploding: public Pickup
{

protected:

public:

	// Overridden to explode on death
	virtual void Die(bool instant = false);

	// Explodes!
	virtual void Explode();

};

/*
	class Pickup_Health
	Gives health to the slug when picked up
*/

class Pickup_Health : public Pickup
{

protected:

public:
	
	virtual void OnCollideWithObject(Object* object);

};

/*
	class Pickup_Weapon
	Gives a weapon to the slug when picked up
*/

class Pickup_Weapon : public Pickup
{

protected:

	WeaponType		type;
	int				ammo;

public:

	// Constructors
	Pickup_Weapon();
	Pickup_Weapon(WeaponType weaponType, int ammoCount);

	virtual void OnCollideWithObject(Object* object);

};