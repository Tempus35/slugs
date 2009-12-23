#pragma once

#include "vec2.h"
#include "box.h"
#include "terrain.h"

/*
	class PhysicsObject
	Base class for all objects effected by physics
*/

#define PHYSICS_LIMIT_SPEED 2000							// Maximum number of pixels/sec an object can move

class PhysicsObject
{

protected:

	Box			bounds;
	Vec2f		velocity;
	Vec2f		acceleration;

	bool		atRest;
	bool		affectedByGravity;
	bool		affectedByWind;

public:

	//
	// Initialization
	//

	PhysicsObject();
	~PhysicsObject();

	//
	// Simulation
	//
	virtual bool Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind);
	virtual void Moved() {};

	//
	// Accessors
	//

	// Gets the position of the object
	const Vec2f& GetPosition() const;

	// Gets the bounding box extents of the object
	const Vec2f& GetExtents() const;

	// Gets the bounding box of the object
	const Box& GetBounds() const;

	Vec2f GetVelocity();
	Vec2f GetAcceleration();
	bool IsAtRest();
	bool IsAffectedByGravity();
	bool IsAffectedByWind();

	virtual void SetAtRest(bool state);
	virtual void SetPosition(Vec2f newPosition);
	virtual void SetPosition(float x, float y);
	virtual void SetExtents(const Vec2f& extents);
	virtual void SetVelocity(Vec2f newVelocity);
	virtual void SetVelocity(float x, float y);
	virtual void SetAcceleration(Vec2f newVelocity);
	virtual void SetAcceleration(float x, float y);
	virtual void SetAffectedByGravity(bool state);
	virtual void SetAffectedByWind(bool state);

};