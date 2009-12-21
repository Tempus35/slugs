#include "projectile.h"

//
// Simulation
//

bool Projectile::Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind)
{

	if (timer > 0.0f)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
			Explode(terrain);

	}

	return Object::Update(elapsedTime, terrain, gravity, wind);

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

	Die();

	World::Get()->SimulateExplosion(pix, -piy, strength);

}

//
// Accessors
//

Object* Projectile::Owner()
{

	return owner;

}

float Projectile::Timer()
{

	return timer;

}

int Projectile::Strength()
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
