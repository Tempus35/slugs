#pragma once

#define SLUGOBJECT_MAX_NAME_CHARS 25

#include "world.h"
#include "gameobject.h"
#include "weaponobject.h"

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
#define SLUG_DEATH_TIMER		2.0f
#define SLUG_EXPLOSION_STRENGTH 30
#define SHOT_POWER_CHARGE_RATE	1.0f
#define SHOT_POWER_MULTIPLIER	2000.0f

class SlugObject : public GameObject
{

private:

	char name[SLUGOBJECT_MAX_NAME_CHARS];
	int movementDirection;
	FaceDirection facingDirection;
	float viewAngle;										// View/aiming angle, -PI/2 <= x <= PI/2
	float power;											// Shot power 0 <= x <= 1
	bool charging;											// Currently charging a shot?
	float death;

public:
	SlugObject();

	virtual bool Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind);
	void StartMovingLeft();
	void StartMovingRight();
	void StopMoving();
	void StartAimingUp();
	void StartAimingDown();
	void StopAiming();
	void StartCharging();
	void Jump();
	void Fire();
	void Die();
	void Explode();

	float ViewAngle();
	FaceDirection FacingDirection();

};