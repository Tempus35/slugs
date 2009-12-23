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

	bool					selected;		// Is this object currently selected

	Box						baseBox;		// Bounding box used for collisions with the terrain

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
	virtual void SetPosition(Vec2f newPosition);
	virtual void SetPosition(float x, float y);
	virtual void SetHitpoints(int newHitpoints);

	//
	// Simulation
	//
	virtual void Moved();
	virtual void Die(bool instant = false);						

	void Select();
	void Deselect();
	bool Contains(float x, float y) const;

	virtual void StartMovingLeft() {};
	virtual void StartMovingRight() {};
	virtual void StopMoving() {};
	virtual void StartAimingUp() {};
	virtual void StartAimingDown() {};
	virtual void StopAiming() {};
	virtual void StartCharging() {};
	virtual void Jump() {};
	virtual void Fire() {};

	// Fired when the object collides with the terrain. Object should return true if it must be repositioned to the point of collision.
	virtual bool OnCollideWithTerrain();

	// Fired when the object collides with another object
	virtual void OnCollideWithObject(Object* object);

	// Sets the collision bounding box of the object
	virtual void SetBounds(float halfWidth, float halfHeight);

	// Gets the base box
	virtual const Box& GetBaseBox() const;

	// Is the object invulnerable?
	virtual bool IsInvulnerable() const;

	//	
	// Accessors
	//

	bool IsAlive() const;
	int GetHitPoints() const;
	const Sprite& GetSprite() const;
	ObjectType GetType() const;

	void AdjustHitpoints(int change);

};