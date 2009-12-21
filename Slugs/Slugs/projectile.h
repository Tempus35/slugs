#pragma once

#include "world.h"
#include "object.h"

class Projectile : public Object
{

protected:

	Object* owner;
	float timer;
	int strength;

	//
	// Internals
	//

	void Explode(Terrain* terrain);

public:

	//
	// Simulation
	//

	virtual bool Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind);
	virtual void OnCollideWithTerrain(Terrain* terrain);
	virtual void OnCollideWithObject(Terrain* terrain, Object* object);

	//
	// Accessors
	//

	Object* Owner();
	float Timer();
	int Strength();

	void SetOwner(Object* object);
	void SetTimer(float newTime);
	void SetStrength(int newStrength);

};