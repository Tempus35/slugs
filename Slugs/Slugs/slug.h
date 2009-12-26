//---------------------------------------------------------------
//
// Slugs
// slug.h
//
//---------------------------------------------------------------

#pragma once

#include "debug.h"
#include "object.h"
#include "weaponstore.h"
#include "projectile.h"
#include "weapon.h"
#include "gravestone.h"

/*
	Enumeration of possible facing directions
*/

enum FaceDirection
{

	FACINGDIRECTION_LEFT,
	FACINGDIRECTION_RIGHT,

};

/*
	Enumeration of possible movement directions
*/

enum MovementDirection
{

	MOVEMENTDIRECTION_NONE		= 0,			// Not moving
	MOVEMENTDIRECTION_LEFT		= 1,			// Moving left
	MOVEMENTDIRECTION_RIGHT		= 2,			// Moving right
	MOVEMENTDIRECTION_UP		= 4,			// Aiming up
	MOVEMENTDIRECTION_DOWN		= 8,			// Aiming down

};

/*
	Forward Declarations
*/

class Team;

/*

	class Slug
	A self contained slug!

	NOTE:	To check if a slug is alive, use GetHitPoints() > 0 rather than IsAlive()
	.		The alive flag is still set to true as a slug is dying!

*/

class Slug : public Object
{

private:

	int				movementDirection;								// Current movement direction
	FaceDirection	facingDirection;								// Current facing direction
	float			aimAngle;										// View/aiming angle, -PI/2 <= x <= PI/2

	Weapon*			currentWeapon;									// Pointer to the currently selected weapon
	WeaponStore*	weaponStore;									// Pointer to the weapon store for this slug
	bool			ownsWeaponStore;								// Does this slug own the weapon store instance?
	float			power;											// Shot power 0 <= x <= 1
	bool			charging;										// Currently charging a shot?

	Team*			team;											// Pointer to the owning team

	float			death;											// Death timer

public:

	// Constructor
	Slug(Team* _team);

	// Destructor
	~Slug();

	// Main update, all processing is done here
	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

	// Starts the slug moving left
	void StartMovingLeft();
	
	// Starts the slug moving right
	void StartMovingRight();

	// Stops any current movement
	void StopMoving();

	// Starts aiming upwards
	void StartAimingUp();

	// Starts aiming downwards
	void StartAimingDown();

	// Stops any aiming movement
	void StopAiming();

	// Begins charging the current weapon
	void StartCharging();

	// Causes the slug to jump in the current facing direction
	void Jump();

	// Fires the current weapon
	void Fire();

	// Kills the slug
	void Die(bool instant = false);

	// Causes the slug to explode
	void Explode();

	// Spawns a gravestone at the current position
	void SpawnGravestone() const;

	// Gets the current facing direction
	FaceDirection GetFacingDirection() const;

	// Gets the current aim angle
	float GetAimAngle() const;

	// Adjust the aim angle by an amount, used by weapons like the machinegun
	void AdjustAim(float amount);

	// Gets the current charge level
	float GetPower() const;

	// Arms the slug with the first available weapon in its weapon store
	void ArmSelf();

	// Arms the slug with the given weapon type
	void ArmSelf(WeaponType type);
	
	// Sets the weapons store available to the slug
	void SetWeapons(WeaponStore* store, bool slugOwns = false);

	// Gets the spawn point for projectiles
	Vec2f GetWeaponPoint() const;

	// Gets the team to which the slug is assigned
	Team* GetTeam() const;

};