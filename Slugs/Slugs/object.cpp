#include "object.h"

//
// Initialization
//

Object::Object()
{

	alive = true;
	hps = 1;

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
	marker.SetImage(image);
	marker.SetScale(1.0f / 16.0f, 1.0f / 16.0f);
	marker.SetColor(Color(255, 0, 0));

}

void Object::SetPosition(Vector2 v)
{
	
	PhysicsObject::SetPosition(v);

	Moved();

}

void Object::SetHitpoints(int newHitpoints)
{

	hps = newHitpoints;

	if (hps <= 0)
		Die();

}

void Object::SetRadius(int newRadius)
{

	radius = newRadius;

}

void Object::Moved()
{

	Vector2 size = sprite.GetSize();
	sprite.SetPosition(fastroundf(position.x - 0.5f * size.x), fastroundf(position.y - 0.5f * size.y));
	marker.SetPosition(fastroundf(position.x), fastroundf(position.y));

}

void Object::Die()
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

bool Object::Contains(int x, int y)
{

	int dx = x - (int)position.x;
	int dy = y - (int)position.y;

	if (dx * dx + dy * dy < radius * radius)
		return true;

	return false;

}

bool Object::IsAlive()
{

	return alive;

}

int Object::HitPoints()
{

	return hps;

}

Sprite& Object::Sprite()
{

	return sprite;

}

int Object::Radius()
{

	return radius;

}

ObjectType Object::Type()
{

	return type;

}


void Object::AdjustHitpoints(int change)
{

	hps += change;

	if (hps <= 0)
		Die();

}
