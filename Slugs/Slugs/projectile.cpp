//---------------------------------------------------------------
//
// Slugs
// projectile.cpp
//
//---------------------------------------------------------------

#include "projectile.h"

#include "game.h"

/*
	class Projectile
*/

Projectile::Projectile(Object* creator) : Object(ObjectType_Projectile)
{

	ASSERT(creator);

	owner = creator;

}

bool Projectile::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	//
	// Decrease our detonation timer if we have one
	//

	if (timer > 0.0f)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
			OnDetonationTimer();

	}

	return Object::Update(elapsedTime, gravity, wind);

}

void Projectile::OnDetonationTimer()
{

	Die();

}

bool Projectile::OnCollideWithTerrain()
{

	Die();

	return true;

}

void Projectile::OnCollideWithObject(Object* object)
{

	if (object != owner)
		Die();

}

void Projectile::Die(bool instant)
{

	alive = false;

	if (!instant)
		Explode();

}

void Projectile::Explode()
{

	// Simulate the explosion on the world
	Game::Get()->GetWorld()->SimulateExplosion(bounds.center, strength);

}

Object* Projectile::GetOwner() const
{

	return owner;

}

float Projectile::GetTimer() const
{

	return timer;

}

float Projectile::GetStrength() const
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

void Projectile::SetStrength(float newStrength)
{

	strength = newStrength;

}

/*
	class Projectile_Bazooka
*/

Projectile_Bazooka::Projectile_Bazooka(Object* creator) : Projectile(creator)
{

}

void Projectile_Bazooka::UpdateOrientation()
{

	// Update orientation to match velocity
	Vec2f direction = velocity.Normalize();
	sprite.SetOrientation(direction);

}

void Projectile_Bazooka::SetVelocity(const Vec2f& newVelocity)
{

	Projectile::SetVelocity(newVelocity);

	UpdateOrientation();

}

bool Projectile_Bazooka::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	bool result = Projectile::Update(elapsedTime, gravity, wind);

	UpdateOrientation();

	return result;

}

/*
	class Projectile_Grenade
*/

Projectile_Grenade::Projectile_Grenade(Object* creator) : Projectile(creator)
{

	tumble = 0.0f;
	bounceCoefficient = 0.4f;

}

bool Projectile_Grenade::OnCollideWithTerrain()
{

	// Use the default object implementation
	return Object::OnCollideWithTerrain();
	
}

void Projectile_Grenade::OnCollideWithObject(Object* object)
{



}

bool Projectile_Grenade::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	bool result = Projectile::Update(elapsedTime, gravity, wind);

	const float rotationRate = 360.0f;

	// Tumble while we are in the air
	if (!atRest)
	{

		if (velocity.x > 0)
			tumble -= elapsedTime * rotationRate;
		else if (velocity.x < 0)
			tumble += elapsedTime * rotationRate;
			
		sprite.SetRotation(tumble);

	}

	return result;

}

/*
	class Projectile_Mine
*/

Projectile_Mine::Projectile_Mine(Object* creator, float armTime, float dudChance) : Projectile(creator)
{

	armTimer = armTime;

	float pick = Random::RandomFloat(0.0f, 1.0f);

	if (pick < dudChance)
		dud = true;
	else
		dud = false;

}

bool Projectile_Mine::OnCollideWithTerrain()
{

	// Use the default object implementation
	return Object::OnCollideWithTerrain();

}

void Projectile_Mine::OnCollideWithObject(Object* object)
{

	if ((!dud) && (armTimer <= 0.0f))
	{


		// Mines only explode on contact with slugs
		if (object->GetType() == ObjectType_Slug)
			Die();

	}

}

bool Projectile_Mine::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	if (armTimer > 0.0f)
		armTimer -= elapsedTime;

	return Projectile::Update(elapsedTime, gravity, wind);

}