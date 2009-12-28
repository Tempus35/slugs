//---------------------------------------------------------------
//
// Slugs
// physicsobject.h
//
//---------------------------------------------------------------

#pragma once

#include "vec2.h"
#include "box.h"
#include "terrain.h"

/*
	class PhysicsObject
	Base class for all objects affected by physics
*/

class PhysicsObject
{

protected:

	Boxf		bounds;										// Collision bounding box of the object
	Vec2f		velocity;									// Velocity of the object (pixels/second)
	Vec2f		acceleration;								// Acceleration of the object (pixels/second squared)

	bool		atRest;										// Is the object at rest? - At rest objects are not updated by the physics system
	bool		affectedByGravity;							// Is this object affected by gravity?
	bool		affectedByWind;								// Is this object affected by wind?

	float		initialHeight;								// Height of the object at the last time it left the rest state

public:

	// Constructor
	PhysicsObject();

	// Main update
	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);

	// Functions that can be overriden if we need to do something when the object moves
	virtual void Moved() {};

	// Gets the position of the object
	const Vec2f& GetPosition() const;

	// Sets the position of the object in world space
	virtual void SetPosition(Vec2f newPosition);
	virtual void SetPosition(float x, float y);			// Deprecated, do not use

	// Gets the bounding box extents of the object
	const Vec2f& GetExtents() const;

	// Sets the bounding box extents of the object
	virtual void SetExtents(const Vec2f& extents);

	// Gets the bounding box of the object
	const Boxf& GetBounds() const;

	// Gets the velocity of the object
	const Vec2f& GetVelocity() const;

	// Gets the speed of the object
	float GetSpeed();

	// Sets the velocity of the object
	virtual void SetVelocity(Vec2f newVelocity);
	virtual void SetVelocity(float x, float y);			// Deprecated, do not use
	
	// Gets the acceleration of the object
	const Vec2f& GetAcceleration() const;

	// Sets the acceleration of the object
	virtual void SetAcceleration(Vec2f newVelocity);
	virtual void SetAcceleration(float x, float y);		// Deprecated, do not use

	// True if the object is at rest
	bool IsAtRest() const;

	// Sets the resting state
	virtual void SetAtRest(bool state = true);

	// True if the object is affected by gravity
	bool IsAffectedByGravity() const;

	// Sets if the object is affected by gravity
	virtual void SetAffectedByGravity(bool state);

	// True if the object is affected by wind
	bool IsAffectedByWind() const;

	// Sets if the object is affected by wind
	virtual void SetAffectedByWind(bool state);

	// Returns the distance which the object last fell
	virtual float GetFallDistance() const;

};