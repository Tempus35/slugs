//---------------------------------------------------------------
//
// Slugs
// projectile.h
//
//---------------------------------------------------------------

#pragma once

#include "object.h"

/*
	class Projectile
	Base class for all projectiles - the base implementation is a standard rocket like projectile
*/

class Projectile : public Object
{

protected:

	Object*			owner;					// Pointer to the object which created the projectile
	float			timer;					// Time in seconds remaining until the projectile detonates
	int				strength;				// Strength of the projectile, used to calculate damage

protected:

	// Constructor
	Projectile(Object* creator);

	// Causes the projectile to explode, damage terrain and units in the vicinity
	virtual void Explode(Terrain* terrain);

public:

	// Updates the projectile based on delta time
	virtual bool Update(float elapsedTime, Terrain* terrain, Vector2& gravity, Vector2& wind);

	// Fired when the detonation timer runs out
	virtual void OnDetonationTimer(Terrain* terrain);

	// Fired when the projectile collides with terrain
	virtual void OnCollideWithTerrain(Terrain* terrain);

	// Fired when the projectile collides with another bject
	virtual void OnCollideWithObject(Terrain* terrain, Object* object);

	// Gets the object which created the projectile
	Object* GetOwner() const;

	// Gets the amount of timer remaining until the projectile detonates
	float GetTimer() const;

	// Gets the strength of the projectile
	int GetStrength() const;

	// Sets the owner of the projectile
	void SetOwner(Object* object);

	// Sets the detonation timer
	void SetTimer(float newTime);

	// Sets the strength of the projectile
	void SetStrength(int newStrength);

};

/*
	class Projectile_Bazooka
	A rocket fired by Weapon_Bazooka. Detonates on impact with terrain.
*/

class Projectile_Bazooka : public Projectile
{

protected:

public:

	Projectile_Bazooka(Object* creator);

};

/*
	class Projectile_Grenade
	A thrown grenade, fired by Weapon_Grenade. Bounces.
*/

class Projectile_Grenade : public Projectile
{

protected:

public:

	Projectile_Grenade(Object* creator);

};