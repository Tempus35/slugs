#include "physicsobject.h"

//
// Initialization
//

PhysicsObject::PhysicsObject()
{

	velocity = Vec2f(0, 0);
	acceleration = Vec2f(0, 0);

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

bool PhysicsObject::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
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

		// Limit velocity
		if (velocity.Length() > PHYSICS_LIMIT_SPEED)
			velocity = velocity.Normalize() * PHYSICS_LIMIT_SPEED;

		// Update position
		int lastX = RoundDownToInt(bounds.center.x), lastY = RoundDownToInt(bounds.center.y);
		bounds.center += velocity * elapsedTime;

		// Return true if object moved
		if ((RoundDownToInt(bounds.center.x) != lastX) || (RoundDownToInt(bounds.center.y) != lastY))
		{
		
			Moved();
			return true;

		}
		else
		{

			bounds.center.x = (float)lastX;
			bounds.center.y = (float)lastY;

		}

	}

	// Object did not move, return false
	return false;

}

//
// Accessors
//

const Vec2f& PhysicsObject::GetPosition() const
{

	return bounds.center;

}

const Vec2f& PhysicsObject::GetExtents() const
{

	return bounds.extents;

}

const Box& PhysicsObject::GetBounds() const
{

	return bounds;

}

Vec2f PhysicsObject::GetVelocity()
{

	return velocity;

}

Vec2f PhysicsObject::GetAcceleration()
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

void PhysicsObject::SetPosition(Vec2f newPosition)
{
	
	bounds.center = newPosition;
	
}

void PhysicsObject::SetPosition(float x, float y)
{

	SetPosition(Vec2f(x, y));

}

void PhysicsObject::SetVelocity(Vec2f newVelocity)
{

	velocity = newVelocity;

	if ((atRest) && (velocity.LengthSquared() > 0.0f))
		atRest = false;

}

void PhysicsObject::SetVelocity(float x, float y)
{

	SetVelocity(Vec2f(x, y));

}


void PhysicsObject::SetAcceleration(Vec2f newAcceleration)
{

	acceleration = newAcceleration;

	if ((atRest) && (acceleration.LengthSquared() > 0.0f))
		atRest = false;

}

void PhysicsObject::SetAcceleration(float x, float y)
{

	SetAcceleration(Vec2f(x, y));

}

void PhysicsObject::SetAffectedByGravity(bool state)
{

	affectedByGravity = state;

}

void PhysicsObject::SetAffectedByWind(bool state)
{

	affectedByWind = state;

}

void PhysicsObject::SetExtents(const Vec2f& extents)
{

	bounds.extents = extents;

}