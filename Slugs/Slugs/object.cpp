#include "object.h"

//
// Initialization
//

Object::Object(ObjectType t)
{

	type = t;

	alive = true;
	hps = 1;
	invulnerable = false;

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

bool Object::Contains(float x, float y) const
{

	return bounds.Contains(x, y);

}

bool Object::OnCollideWithTerrain()
{

	// Stop moving
	SetAtRest(true);

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

const Sprite& Object::GetSprite() const
{

	return sprite;

}

ObjectType Object::GetType() const
{

	return type;

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
