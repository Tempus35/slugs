#pragma once

#include "debug.h"
#include "world.h"
#include "object.h"
#include "weaponstore.h"
#include "projectile.h"
#include "weapon.h"

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
#define SLUG_MAX_UP_STEP		5							// Maximum number of pixels that can be moved vertically per step
#define SLUG_DEATH_TIMER		2.0f						// Time between reaching 0 hps and exploding
#define SLUG_EXPLOSION_STRENGTH 30							// Strength of the slug death explosion
#define SHOT_POWER_CHARGE_RATE	1.0f						// Rate of weapon charge
#define SHOT_POWER_MULTIPLIER	2000.0f						// Multiplier to turn shot power into shot speed				

class Team;

class Slug : public Object
{

private:

	int				movementDirection;
	FaceDirection	facingDirection;								
	float			viewAngle;										// View/aiming angle, -PI/2 <= x <= PI/2

	Weapon*			currentWeapon;									// Pointer to the currently selected weapon
	WeaponStore*	weaponStore;									// Pointer to the weapon store for this slug
	float			power;											// Shot power 0 <= x <= 1
	bool			charging;										// Currently charging a shot?

	Team*			team;											// Pointer to the owning team

	float			death;											// Death timer

public:

	Slug();

	virtual bool Update(float elapsedTime, Terrain* terrain, const Vector2& gravity, const Vector2& wind);
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
	void Die();

	// Causes the slug to explode
	void Explode();

	// Gets the current aim angle
	float ViewAngle();

	// Gets the current facing directioon
	FaceDirection FacingDirection();

	// Arms the slug with the first available weapon in its weapon store
	void ArmSelf();
	
	// Sets the weapons store available to the slug
	void SetWeapons(WeaponStore* store);

	// Gets the team to which the slug is assigned
	Team* GetTeam() const;

};