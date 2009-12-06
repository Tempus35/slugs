#pragma once

#include <SFML/graphics.hpp>

#include "physicsobject.h"
#include "fastmath.h"
#include "sprite.h"

/*
	enum GameObjectType
*/

enum GameObjectType
{

	GAMEOBJECTTYPE_UNKNOWN,
	GAMEOBJECTTYPE_SLUG,

};

/*
	class GameObject
*/

class GameObject : public PhysicsObject
{

protected:

	GameObjectType type;

	bool alive;								// Is this object alive (dead objects are removed by the world)
	int hps;								// Hit points

	Sprite sprite;							// Sprite instance for the object

	int radius;

	bool selected;

public:

	Sprite marker;

	//
	// Initialization
	//

	GameObject();
	~GameObject();

	//
	// Setup
	//

	virtual void SetImage(ImageResource* image);
	virtual void SetPosition(Vector2 newPosition);
	virtual void SetHitpoints(int newHitpoints);
	virtual void SetRadius(int newRadius);

	//
	// Simulation
	//
	virtual void Moved();
	virtual void Die();						// Kills the object

	void Select();
	void Deselect();
	bool Contains(int x, int y);

	virtual void StartMovingLeft() {};
	virtual void StartMovingRight() {};
	virtual void StopMoving() {};
	virtual void StartAimingUp() {};
	virtual void StartAimingDown() {};
	virtual void StopAiming() {};
	virtual void StartCharging() {};
	virtual void Jump() {};
	virtual void Fire() {};

	virtual void OnCollideWithTerrain(Terrain* terrain) {};
	virtual void OnCollideWithObject(Terrain* terrain, GameObject* object) {};

	//	
	// Accessors
	//

	bool IsAlive();
	int HitPoints();
	Sprite& Sprite();
	int Radius();
	GameObjectType Type();

	void AdjustHitpoints(int change);

};