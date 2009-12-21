#pragma once

#include <string>
#include <SFML/graphics.hpp>

#include "physicsobject.h"
#include "fastmath.h"
#include "sprite.h"

/*
	enum ObjectType
*/

enum ObjectType
{

	OBJECTTYPE_UNKNOWN,
	OBJECTTYPE_SLUG,

};

/*
	class Object
*/

class Object : public PhysicsObject
{

protected:

	ObjectType					type;		// Object type
	std::string						name;		// Object name

	bool							alive;		// Is this object alive (dead objects are removed by the world)
	int								hps;		// Hit points

	Sprite sprite;								// Sprite instance for the object

	int radius;									// Collision radius

	bool selected;								// Is this object currently selected

public:

	Sprite marker;

	//
	// Initialization
	//

	Object();
	~Object();

	// Name accessors
	const std::string& GetName() const;
	void SetName(const std::string& newName);

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
	virtual void OnCollideWithObject(Terrain* terrain, Object* object) {};

	//	
	// Accessors
	//

	bool IsAlive();
	int HitPoints();
	Sprite& Sprite();
	int Radius();
	ObjectType Type();

	void AdjustHitpoints(int change);

};