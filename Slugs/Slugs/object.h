#pragma once

#include <string>
#include <SFML/graphics.hpp>

#include "physicsobject.h"
#include "fastmath.h"
#include "sprite.h"

/*
	Enumeration of all object types
*/

enum ObjectType
{

	ObjectType_Unknown,
	ObjectType_Slug,
	ObjectType_Projectile,
	ObjectType_Gravestone,

};

/*
	class Object
	Base class for all game objects. An object is something that is physically present in the world.
*/

class Object : public PhysicsObject
{

private:

	ObjectType				type;			// Object type

protected:

	std::string				name;			// Object name

	bool					alive;			// Is this object alive (dead objects are removed by the world)
	int						hps;			// Hit points
	bool					invulnerable;	// Is this object immune to harm

	Sprite					sprite;			// Sprite instance for the object

	int						radius;			// Collision radius

	bool					selected;		// Is this object currently selected

protected:

	// Constructor
	Object(ObjectType t);

public:

	// Debug marker
	Sprite marker;

public:

	// Initialization
	~Object();

	// Name accessors
	const std::string& GetName() const;
	void SetName(const std::string& newName);

	//
	// Setup
	//

	virtual void SetImage(ImageResource* image);
	virtual void SetPosition(Vector2 newPosition);
	void SetPosition(float x, float y);
	virtual void SetHitpoints(int newHitpoints);
	virtual void SetRadius(int newRadius);

	//
	// Simulation
	//
	virtual void Moved();
	virtual void Die();						

	void Select();
	void Deselect();
	bool Contains(int x, int y) const;

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

	bool IsAlive() const;
	int GetHitPoints() const;
	const Sprite& GetSprite() const;
	int GetRadius() const;
	ObjectType GetType() const;

	void AdjustHitpoints(int change);

};