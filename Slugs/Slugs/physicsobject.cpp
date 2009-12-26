//---------------------------------------------------------------
//
// Slugs
// physicsobject.cpp
//
//---------------------------------------------------------------

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

bool PhysicsObject::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	const float PHYSICS_LIMIT_SPEED = 2000.0f;	// Absolute maximum speed in pixels/second

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

		// Determine if we actually moved one or more pixels
		if ((RoundDownToInt(bounds.center.x) != lastX) || (RoundDownToInt(bounds.center.y) != lastY))
		{
		
			// We moved
			Moved();

			// Returning true indicates that the object moved
			return true;

		}
		else
		{

			// Set back to the last integer position
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

void PhysicsObject::SetPosition(Vec2f newPosition)
{
	
	bounds.center = newPosition;
	
}

void PhysicsObject::SetPosition(float x, float y)
{

	SetPosition(Vec2f(x, y));

}

const Vec2f& PhysicsObject::GetExtents() const
{

	return bounds.extents;

}

void PhysicsObject::SetExtents(const Vec2f& extents)
{

	bounds.extents = extents;

}

const Box& PhysicsObject::GetBounds() const
{

	return bounds;

}

const Vec2f& PhysicsObject::GetVelocity() const
{

	return velocity;

}

void PhysicsObject::SetVelocity(float x, float y)
{

	SetVelocity(Vec2f(x, y));

}

void PhysicsObject::SetVelocity(Vec2f newVelocity)
{

	velocity = newVelocity;

	if ((atRest) && (velocity.LengthSquared() > 0.0f))
		atRest = false;

}

const Vec2f& PhysicsObject::GetAcceleration() const
{

	return acceleration;

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

bool PhysicsObject::IsAtRest() const
{

	return atRest;

}

void PhysicsObject::SetAtRest(bool state)
{

	atRest = state;

	if (atRest)
		velocity.x = velocity.y = acceleration.x = acceleration.y = 0.0f;
	else
		initialHeight = bounds.center.y;

}

bool PhysicsObject::IsAffectedByGravity() const
{

	return affectedByGravity;

}

void PhysicsObject::SetAffectedByGravity(bool state)
{

	affectedByGravity = state;

}

bool PhysicsObject::IsAffectedByWind() const
{

	return affectedByWind;

}

void PhysicsObject::SetAffectedByWind(bool state)
{

	affectedByWind = state;

}

float PhysicsObject::GetFallDistance() const
{

	return Max(initialHeight - bounds.center.y, 0.0f);

}