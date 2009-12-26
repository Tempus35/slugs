//---------------------------------------------------------------
//
// Slugs
// projectile.h
//
//---------------------------------------------------------------

#pragma once

#include "object.h"
#include "fxmanager.h"

/*
	class Projectile
	Base class for all projectiles - the base implementation is a standard rocket like projectile
*/

class Projectile : public Object
{

protected:

	float			timer;					// Time in seconds remaining until the projectile detonates
	float			strength;				// Strength of the projectile, used to calculate damage

protected:

	// Constructor
	Projectile(Object* creator);

	// Causes the projectile to explode, damage terrain and units in the vicinity
	virtual void Explode();

public:

	// Updates the projectile based on delta time
	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

	// Fired when the detonation timer runs out
	virtual void OnDetonationTimer();

	// Fired when the projectile collides with terrain
	virtual bool OnCollideWithTerrain();

	// Fired when the projectile collides with another bject
	virtual void OnCollideWithObject(Object* object);

	// Kills the projectile
	virtual void Die(bool instant = false);	
	
	// Gets the amount of timer remaining until the projectile detonates
	virtual float GetTimer() const;

	// Gets the strength of the projectile
	virtual float GetStrength() const;

	// Sets the detonation timer
	virtual void SetTimer(float newTime);

	// Sets the strength of the projectile
	virtual void SetStrength(float newStrength);

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

	// Updates the orientation of the projectile to match its velocity
	virtual void UpdateOrientation();

	virtual void SetVelocity(const Vec2f& newVelocity);

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

};

/*
	class Projectile_Grenade
	A thrown grenade, fired by Weapon_Grenade. Bounces.
*/

class Projectile_Grenade : public Projectile
{

protected:

	float tumble;

public:

	Projectile_Grenade(Object* creator);

	virtual bool OnCollideWithTerrain();
	virtual void OnCollideWithObject(Object* object);

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

};

/*
	class Projectile_Mine
	A mine that which stays on the ground until triggered
*/

class Projectile_Mine : public Projectile
{

protected:

	bool			dud;						// Is this mine a dud?
	float			armTimer;					// Arming timer

public:

	Projectile_Mine(Object* creator, float armTime, float dudChance);

	virtual bool OnCollideWithTerrain();
	virtual void OnCollideWithObject(Object* object);

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

};