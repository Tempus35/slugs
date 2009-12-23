#pragma once

#include "debug.h"
#include "object.h"
#include "weaponstore.h"
#include "projectile.h"
#include "weapon.h"
#include "gravestone.h"

enum FaceDirection
{

	FACINGDIRECTION_LEFT,
	FACINGDIRECTION_RIGHT,

};

enum MovementDirection
{

	MOVEMENTDIRECTION_NONE		= 0,
	MOVEMENTDIRECTION_LEFT		= 1,
	MOVEMENTDIRECTION_RIGHT		= 2,
	MOVEMENTDIRECTION_UP		= 4,
	MOVEMENTDIRECTION_DOWN		= 8,

};

#define SLUG_MOVEMENT_SPEED		50.0f						// Pixels/sec
#define SLUG_AIM_SPEED			45.0f * PI_OVER_180			// Radians/sec 
#define SLUG_MAX_UP_STEP		5							// Maximum number of pixels that can be moved up per step
#define SLUG_MAX_DOWN_STEP		5							// Maximum number of pixels that can be move down per step
#define SLUG_DEATH_TIMER		2.0f						// Time between reaching 0 hps and exploding
#define SLUG_EXPLOSION_STRENGTH 30.0f							// Strength of the slug death explosion
#define SHOT_POWER_CHARGE_RATE	1.0f						// Rate of weapon charge		

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

	Slug(Team* _team);
	~Slug();

	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);
	void StartMovingLeft();
	void StartMovingRight();
	void StopMoving();
	void StartAimingUp();
	void StartAimingDown();
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