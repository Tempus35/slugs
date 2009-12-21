#pragma once

#include "vector2.h"
#include "terrain.h"

/*
	class PhysicsObject
	Base class for all objects effected by physics
*/

class PhysicsObject
{

protected:

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	bool atRest;
	bool affectedByGravity;
	bool affectedByWind;

public:

	//
	// Initialization
	//

	PhysicsObject();
	~PhysicsObject();

	//
	// Simulation
	//
	virtual bool Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind);
	virtual void Moved() {};

	//
	// Accessors
	//

	Vector2 GetPosition();
	Vector2 GetVelocity();
	Vector2 GetAcceleration();
	bool IsAtRest();
	bool IsAffectedByGravity();
	bool IsAffectedByWind();

	virtual void SetAtRest(bool state);
	virtual void SetPosition(Vector2 newPosition);
	virtual void SetPosition(float x, float y);
	virtual void SetVelocity(Vector2 newVelocity);
	virtual void SetVelocity(float x, float y);
	virtual void SetAcceleration(Vector2 newVelocity);
	virtual void SetAcceleration(float x, float y);
	virtual void SetAffectedByGravity(bool state);
	virtual void SetAffectedByWind(bool state);

};