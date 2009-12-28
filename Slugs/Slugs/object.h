//---------------------------------------------------------------
//
// Slugs
// object.h
//
//---------------------------------------------------------------

#pragma once

#include <string>
#include <SFML/graphics.hpp>

#include "physicsobject.h"
#include "fastmath.h"
#include "sprite.h"

/*
	Enumeration of all object types
	These are base types
*/

enum ObjectType
{

	ObjectType_Any				= -1,
	ObjectType_Unknown,
	ObjectType_Slug,			// A slug
	ObjectType_Projectile,		// A projectile such as a grenade
	ObjectType_Flavor,			// A flavor object such as a gravestone, these are generally invulnerable to damage
	ObjectType_Pickup,			// An object that can be picked up by slugs such as a crate

};

/*
	class Object
	Base class for all game objects. An object is something that is physically present in the world.
*/

class Object : public PhysicsObject
{

private:

	ObjectType				type;				// Object type

protected:

	std::string				name;				// Object name
	Object*					owner;				// Pointer to the object which created the object

	bool					alive;				// Is this object alive (dead objects are removed by the world)
	int						hps;				// Hit points
	bool					invulnerable;		// Is this object immune to harm

	Sprite					sprite;				// Sprite instance for the object

	bool					selected;			// Is this object currently selected

	float					bounceCoefficient;	// Bounciness factor of the object

protected:

	// Constructor
	Object(Object* creator, ObjectType t);

public:

	// Destructor
	~Object();

	// Gets the name of the object
	const std::string& GetName() const;
	
	// Sets the name of the object
	void SetName(const std::string& newName);

	// Gets the object which created the projectile
	virtual Object* GetOwner() const;

	// Returns true if object owns us
	virtual bool IsOwnedBy(Object* object) const;

	// Sets the owner of the projectile
	virtual void SetOwner(Object* object);

	// Sets the image resource used by the object
	virtual void SetImage(ImageResource* image);

	// Sets the position of the object
	virtual void SetPosition(Vec2f newPosition);
	virtual void SetPosition(float x, float y);		// Deprecated, do not use

	// Sets the hit points of the object
	virtual void SetHitpoints(int newHitpoints);

	// Fired when an object moves
	virtual void Moved();
	
	// Causes the object to die. If instant is set, the object should die with no further processing.
	virtual void Die(bool instant = false);						

	// Used to notify the object it has been selected
	void Select();

	// Used to notify the object is has been deselected
	void Deselect();

	// Returns true if the bounds of the object contain the point
	bool Contains(const Vec2f& position) const;

	// Fired when the object collides with the terrain. Object returning true will be set into the rest state.
	virtual bool OnCollideWithTerrain();

	// Fired when the object collides with another object
	virtual void OnCollideWithObject(Object* object);

	// Fired when the slugs hitpoints change
	virtual void OnHitpointsChanged(int oldValue);

	// Sets the collision bounding box of the object
	virtual void SetBounds(float halfWidth, float halfHeight);

	// Is the object invulnerable?
	virtual bool IsInvulnerable() const;

	// Returns true if the object is currently alive. Dead objects are removed from the world.
	virtual bool IsAlive() const;

	// Gets the current number of hit points
	virtual int GetHitPoints() const;

	// Adjust the current hitpoints of the object
	virtual void AdjustHitpoints(int change);

	// Gets the current sprite used by the object
	const Sprite& GetSprite() const;

	// Gets the base type of the object
	ObjectType GetType() const;

	// Renders debugging info for the object
	virtual void DebugRender();

};

std::string ObjectTypeToString(ObjectType type);