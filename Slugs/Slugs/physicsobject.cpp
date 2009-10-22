// -PB

#include "physicsobject.h"

//
// Initialization
//

PhysicsObject::PhysicsObject()
{

	position = Vector2(0, 0);
	velocity = Vector2(0, 0);
	acceleration = Vector2(0, 0);

	atRest = false;
	affectedByGravity = true;
	affectedByWind = false;

}

PhysicsObject::~PhysicsObject()
{

}

//
// Simulation
//

bool PhysicsObject::Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind)
{

	if (!atRest)
	{

		// Update acceleration
		if (affectedByGravity)
			acceleration += gravity * elapsedTime;

		if (affectedByWind)
			acceleration += wind * elapsedTime;

		// Update velocity
		velocity += acceleration * elapsedTime;

		// Update position
		int lastX = (int)position.x, lastY = (int)position.y;
		position += velocity * elapsedTime;

		// Return true if object moved
		if (((int)position.x != lastX) || ((int)position.y != lastY))
		{
		
			Moved();
			return true;

		}

	}

	// Object did not move, return false
	return false;

}

//
// Accessors
//

Vector2 PhysicsObject::Position()
{

	return position;

}

Vector2 PhysicsObject::Velocity()
{

	return velocity;

}

Vector2 PhysicsObject::Acceleration()
{

	return acceleration;

}

bool PhysicsObject::IsAtRest()
{

	return atRest;

}

bool PhysicsObject::IsAffectedByGravity()
{

	return affectedByGravity;

}

bool PhysicsObject::IsAffectedByWind()
{

	return affectedByWind;

}

void PhysicsObject::SetAtRest(bool state)
{

	atRest = state;

	if (atRest)
		velocity.x = velocity.y = acceleration.x = acceleration.y = 0.0f;

}

void PhysicsObject::SetPosition(Vector2 newPosition)
{
	
	position = newPosition;
	
}

void PhysicsObject::SetPosition(float x, float y)
{

	SetPosition(Vector2(x, y));

}

void PhysicsObject::SetVelocity(Vector2 newVelocity)
{

	velocity = newVelocity;

	if ((atRest) && (VectorLengthSquared(velocity) > 0.0f))
		atRest = false;

}

void PhysicsObject::SetVelocity(float x, float y)
{

	SetVelocity(Vector2(x, y));

}


void PhysicsObject::SetAcceleration(Vector2 newAcceleration)
{

	acceleration = newAcceleration;

	if ((atRest) && (VectorLengthSquared(acceleration) > 0.0f))
		atRest = false;

}

void PhysicsObject::SetAcceleration(float x, float y)
{

	SetAcceleration(Vector2(x, y));

}

void PhysicsObject::SetAffectedByGravity(bool state)
{

	affectedByGravity = state;

}

void PhysicsObject::SetAffectedByWind(bool state)
{

	affectedByWind = state;

}