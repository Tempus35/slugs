#include "weaponobject.h"

//
// Simulation
//

bool WeaponObject::Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind)
{

	if (timer > 0.0f)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
			Explode(terrain);

	}

	return GameObject::Update(elapsedTime, terrain, gravity, wind);

}

void WeaponObject::OnCollideWithTerrain(Terrain* terrain)
{

	Explode(terrain);

}

void WeaponObject::OnCollideWithObject(Terrain* terrain, GameObject* object)
{

	if (object != owner)
		Explode(terrain);

}

void WeaponObject::Explode(Terrain* terrain)
{

	int pix = (int)position.x;
	int piy = (int)position.y;

	Die();

	World::Get()->SimulateExplosion(pix, -piy, strength);

}

//
// Accessors
//

GameObject* WeaponObject::Owner()
{

	return owner;

}

float WeaponObject::Timer()
{

	return timer;

}

int WeaponObject::Strength()
{

	return strength;

}

void WeaponObject::SetOwner(GameObject* object)
{

	owner = object;

}

void WeaponObject::SetTimer(float newTime)
{

	timer = newTime;

}

void WeaponObject::SetStrength(int newStrength)
{

	strength = newStrength;

}
