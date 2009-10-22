#pragma once

#include "world.h"
#include "gameobject.h"

class WeaponObject : public GameObject
{

protected:

	GameObject* owner;
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
	virtual void OnCollideWithObject(Terrain* terrain, GameObject* object);

	//
	// Accessors
	//

	GameObject* Owner();
	float Timer();
	int Strength();

	void SetOwner(GameObject* object);
	void SetTimer(float newTime);
	void SetStrength(int newStrength);

};