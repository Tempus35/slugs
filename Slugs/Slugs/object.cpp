//---------------------------------------------------------------
//
// Slugs
// object.cpp
//
//---------------------------------------------------------------

#include "object.h"
#include "game.h"

//
// Initialization
//

Object::Object(Object* creator, ObjectType t)
{

	owner = creator;

	type = t;

	alive = true;
	hps = 1;
	invulnerable = false;

	// Objects don't bounce much by default
	bounceCoefficient = 0.2f;

}

Object::~Object()
{

}

const std::string& Object::GetName() const
{

	return name;

}

void Object::SetName(const std::string& newName)
{

	name = newName;

}

Object* Object::GetOwner() const
{

	return owner;

}

bool Object::IsOwnedBy(Object* object) const
{

	return (owner == object);

}

void Object::SetOwner(Object* object)
{

	owner = object;

}

void Object::SetImage(ImageResource* image)
{

	sprite.SetImage(image);
	sprite.SetCenter((float)image->Image().GetWidth() / 2, (float)image->Image().GetHeight() / 2);
	
	marker.SetImage(image);
	marker.SetScale(1.0f / 16.0f, 1.0f / 16.0f);
	marker.SetColor(Color(255, 0, 0));

}

void Object::SetPosition(Vec2f v)
{
	
	PhysicsObject::SetPosition(v);

	baseBox.center.x = bounds.center.x;
	baseBox.center.y = bounds.center.y - bounds.extents.y;
	baseBox.extents.x = 2.5f;
	baseBox.extents.y = 5 + 1.0f;

	Moved();

}

void Object::SetPosition(float x, float y)
{
	
	PhysicsObject::SetPosition(x, y);

	baseBox.center.x = bounds.center.x;
	baseBox.center.y = bounds.center.y - bounds.extents.y;
	baseBox.extents.x = 2.5f;
	baseBox.extents.y = 5 + 1.0f;

	Moved();

}

void Object::SetHitpoints(int newHitpoints)
{

	hps = newHitpoints;

	if (hps <= 0)
		Die();

}

void Object::Moved()
{

	sprite.SetPosition(bounds.center.x, -bounds.center.y);
	marker.SetPosition(bounds.center.x, -bounds.center.y);

}

void Object::Die(bool instant)
{

	alive = false;

}

void Object::Select()
{

	sprite.SetColor(Color(0, 255, 0));
	selected = true;

}

void Object::Deselect()
{

	sprite.SetColor(Color(255, 255, 255));
	selected = false;

}

bool Object::Contains(const Vec2f& position) const
{

	return bounds.Contains(position);

}

bool Object::OnCollideWithTerrain()
{

	// Get terrain normal at the collision point
	Vec2f normal = Game::Get()->GetWorld()->GetNormalForBox(bounds.center.x, bounds.center.y, Max(bounds.extents.x, 5.0f) * 2.0f, Max(bounds.extents.y, 5.0f) * 2.0f);

	//
	// Bounce
	//

	const float minSpeed = 100.0f;

	float speed = velocity.Length() * bounceCoefficient;

	if (speed > minSpeed)
	{

		Vec2f direction = velocity.Normalize();

		direction -= normal * (DotProduct(normal, direction)) * 2.0f;

		velocity = direction * speed;

		return false;

	}
	else
		return true;

}

void Object::OnCollideWithObject(Object* object)
{

}

void Object::SetBounds(float halfWidth, float halfHeight)
{

	bounds.extents.x = halfWidth;
	bounds.extents.y = halfHeight;

	baseBox.center.x = bounds.center.x;
	baseBox.center.y = bounds.center.y - bounds.extents.y;
	baseBox.extents.x = 2.5f;
	baseBox.extents.y = 5 + 1.0f;

}

const Box& Object::GetBaseBox() const
{

	return baseBox;

}

bool Object::IsInvulnerable() const
{

	return invulnerable;

}

bool Object::IsAlive() const
{

	return alive;

}

int Object::GetHitPoints() const
{

	return hps;

}

void Object::AdjustHitpoints(int change)
{

	if (!invulnerable)
	{

		hps += change;

		if (hps <= 0)
			Die();

	}

}

const Sprite& Object::GetSprite() const
{

	return sprite;

}

ObjectType Object::GetType() const
{

	return type;

}