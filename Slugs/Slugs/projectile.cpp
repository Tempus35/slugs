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
	Game::Get()->GetWorld()->SimulateExplosion(bounds.center.x, bounds.center.y, strength);

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

}

bool Projectile_Grenade::OnCollideWithTerrain()
{

	// Get terrain normal at the collision point
	Vec2f normal = Game::Get()->GetWorld()->GetNormalForBox(bounds.center.x, bounds.center.y, bounds.extents.x * 4.0f, bounds.extents.y * 4.0f);

	//
	// Bounce
	//

	const float bounceCoefficient = 0.40f;
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