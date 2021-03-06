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

class Player;
class Team;
class AIController;

/*

	class Slug
	A self contained slug!

	NOTE:	To check if a slug is alive, use GetHitPoints() > 0 rather than IsAlive()
	.		The alive flag is still set to true as a slug is dying!

*/

class Slug : public Object
{

private:

	int						movementDirection;						// Current movement direction
	FaceDirection			facingDirection;						// Current facing direction
	float					aimAngle;								// View/aiming angle, -PI/2 <= x <= PI/2

	Weapon*					currentWeapon;							// Pointer to the currently selected weapon
	WeaponStore*			weaponStore;							// Pointer to the weapon store for this slug
	bool					ownsWeaponStore;						// Does this slug own the weapon store instance?
	float					power;									// Shot power 0.0f <= x <= 1.0f
	bool					charging;								// Currently charging a shot?

	Team*					team;									// Pointer to the owning team

	float					death;									// Death timer

	AIController*			controller;								// Pointer to our AI controller if we have one

	Object*					goal;									// Pointer to last goal object
		
	float					stunTimer;								// Remaining duration of a stun effect if > 0

public:

	// Constructor
	Slug(Team* _team, AIController* aiController = NULL);

	// Destructor
	~Slug();

	// Main update, all processing is done here
	bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

	// Overridden to apply falling damage
	bool OnCollideWithTerrain(const Vec2f& collisionPoint);

	// Overriden to end the players turn if the slug takes damage
	void OnHitpointsChanged(int oldValue);

	// Starts the slug moving left
	void StartMovingLeft();
	
	// Starts the slug moving right
	void StartMovingRight();

	// Stops any current movement
	void StopMoving();

	// Starts adjusting aim upwards
	void StartAimingUp();

	// Starts adjusting aim downwards
	void StartAimingDown();

	// Starts adjusting aim towards a direction
	bool StartAimingTowards(const Vec2f& direction);

	// Stops any aiming movement
	void StopAiming();

	// Stops all actions in progress
	void StopEverything();

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

	// Gets the current aim direction
	Vec2f GetAimDirection() const;

	// Adjust the aim angle by an amount, used by weapons like the machinegun
	void AdjustAim(float amount);

	// Gets the current charge level
	float GetPower() const;

	// Sets the weapon charge level
	void SetPower(float newPower);

	// Arms the slug with the first available weapon in its weapon store
	Weapon* ArmSelf();

	// Arms the slug with the given weapon type
	Weapon* ArmSelf(WeaponType type);
	
	// Sets the weapons store available to the slug
	void SetWeapons(WeaponStore* store, bool slugOwns = false);

	// Gets the spawn point for projectiles
	Vec2f GetWeaponPoint() const;

	// Gets the team to which the slug is assigned
	Team* GetTeam() const;

	// Gets the player which owns the slug
	Player* GetPlayer() const;

	// Faces the slug to the right
	void FaceRight();

	// Faces the slug to the left
	void FaceLeft();

	// Sets the current goal object
	void SetGoal(Object* object);

	// Renders the slug and associated objects
	void Render();

	// Renders debugging information
	void DebugRender();

	// Stuns the slug for duration seconds. Stunned slugs can't perform any actions.
	void StunSelf(float duration);

	// Sets the target position if the current weapon is targetable
	void SetTarget(const Vec2f& position);

	// Returns true is the slug is the currently active slug of the player whose turn it is
	bool IsActive() const;

	// Transforms an attachment offset into slug space
	Vec2f TransformAttachmentOffset(const Vec2f& offset);

	// Adds a weapon to the slugs weaponstore
	void GiveWeapon(WeaponType weaponType, int ammo);

};