// -PB

#include "gameobject.h"

//
// Initialization
//

GameObject::GameObject()
{

	alive = true;
	hps = 1;

}

GameObject::~GameObject()
{

}

//
// Setup
//

void GameObject::SetImage(ImageResource* image)
{

	sprite.SetImage(image);
	marker.SetImage(image);
	marker.SetScale(1.0f / 16.0f, 1.0f / 16.0f);
	marker.SetColor(Color(255, 0, 0));

}

void GameObject::SetPosition(Vector2 v)
{
	
	PhysicsObject::SetPosition(v);

	Moved();

}

void GameObject::SetHitpoints(int newHitpoints)
{

	hps = newHitpoints;

	if (hps <= 0)
		Die();

}

void GameObject::SetRadius(int newRadius)
{

	radius = newRadius;

}

//
// Simulation
//

void GameObject::Moved()
{

	Vector2 size = sprite.GetSize();
	sprite.SetPosition(fastroundf(position.x - 0.5f * size.x), fastroundf(position.y - 0.5f * size.y));
	marker.SetPosition(fastroundf(position.x), fastroundf(position.y));

}

void GameObject::Die()
{

	alive = false;

}

void GameObject::Select()
{

	sprite.SetColor(Color(0, 255, 0));
	selected = true;

}

void GameObject::Deselect()
{

	sprite.SetColor(Color(255, 255, 255));
	selected = false;

}

bool GameObject::Contains(int x, int y)
{

	int dx = x - (int)position.x;
	int dy = y - (int)position.y;

	if (dx * dx + dy * dy < radius * radius)
		return true;

	return false;

}

//
// Accessors
//

bool GameObject::IsAlive()
{

	return alive;

}

int GameObject::HitPoints()
{

	return hps;

}

Sprite& GameObject::Sprite()
{

	return sprite;

}

int GameObject::Radius()
{

	return radius;

}

GameObjectType GameObject::Type()
{

	return type;

}


void GameObject::AdjustHitpoints(int change)
{

	hps += change;

	if (hps <= 0)
		Die();

}
