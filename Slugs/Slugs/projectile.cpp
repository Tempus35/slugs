//---------------------------------------------------------------
//
// Slugs
// projectile.cpp
//
//---------------------------------------------------------------

#include "projectile.h"

#include "world.h"

/*
	class Projectile
*/

Projectile::Projectile(Object* creator) : Object(ObjectType_Projectile)
{

	ASSERT(creator);

	owner = creator;

}

bool Projectile::Update(float elapsedTime, Terrain* terrain, const Vector2& gravity, const Vector2& wind)
{

	if (timer > 0.0f)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
			OnDetonationTimer(terrain);

	}

	return Object::Update(elapsedTime, terrain, gravity, wind);

}

void Projectile::OnDetonationTimer(Terrain* terrain)
{

	Explode(terrain);

}

void Projectile::OnCollideWithTerrain(Terrain* terrain)
{

	Explode(terrain);

}

void Projectile::OnCollideWithObject(Terrain* terrain, Object* object)
{

	if (object != owner)
		Explode(terrain);

}

void Projectile::Explode(Terrain* terrain)
{

	int pix = (int)position.x;
	int piy = (int)position.y;

	// Kill the projectile
	Die();

	// Simulate the explosion on the world
	World::Get()->SimulateExplosion(pix, -piy, strength);

}

Object* Projectile::GetOwner() const
{

	return owner;

}

float Projectile::GetTimer() const
{

	return timer;

}

int Projectile::GetStrength() const
{

	return strength;

}

void Projectile::SetOwner(Object* object)
{

	owner = object;

}

void Projectile::SetTimer(float newTime)
{

	timer = newTime;

}

void Projectile::SetStrength(int newStrength)
{

	strength = newStrength;

}

/*
	class Projectile_Bazooka
*/

Projectile_Bazooka::Projectile_Bazooka(Object* creator) : Projectile(creator)
{

}

/*
	class Projectile_Grenade
*/

Projectile_Grenade::Projectile_Grenade(Object* creator) : Projectile(creator)
{

}
