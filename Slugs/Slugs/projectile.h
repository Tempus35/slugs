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
	struct ExplosionData
	Holds explosion data used by the world to simulate explosions
*/

struct ExplosionData
{

	float			explosionRadius;		// Radius of the explosion in pixels
	float			forceRadius;			// Radius inside which objects get pushed away
	float			forceStrength;			// Maximum force to apply inside the for radius
	float			damageRadius;			// Radius inside whcih objects are damage
	float			damageStrength;			// Maximum damage inside the explosion radius

	ExplosionData() {};
	ExplosionData(float _explosionRadius, float _forceRadius, float _forceStrength, float _damageRadius, float _damageStrength)
		: explosionRadius(_explosionRadius), forceRadius(_forceRadius), forceStrength(_forceStrength), damageRadius(_damageRadius), damageStrength(_damageStrength) {}

};

/*
	class Projectile
	Base class for all projectiles - the base implementation is a standard rocket like projectile
*/

class Projectile : public Object
{

protected:

	float				timer;					// Time in seconds remaining until the projectile detonates
	ExplosionData		explosionData;			// Explosion info

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

	// Sets the detonation timer
	virtual void SetTimer(float newTime);

	// Gets the current explosion data for the projectile
	const ExplosionData& GetExplosionData() const;

	// Sets the explosion data for the projectile
	void SetExplosionData(const ExplosionData& data);

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

	Projectile_Mine(float armTime, float dudChance);

	virtual bool OnCollideWithTerrain();
	virtual void OnCollideWithObject(Object* object);

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

	virtual void DebugRender();

};

/*
	class Projectile_HomingMissile
	Like a bazooka rocket but homes in on its target
*/

class Projectile_HomingMissile : public Projectile_Bazooka
{

protected:

	Vec2f		target;				// Position of the target
	float		armTimer;			// Time before homing starts

public:

	Projectile_HomingMissile(Object* creator, const Vec2f& targetPosition);

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

};

class Projectile_Dynamite : public Projectile
{

protected:

public:

	Projectile_Dynamite();

	virtual bool OnCollideWithTerrain();
	virtual void OnCollideWithObject(Object* object);

};