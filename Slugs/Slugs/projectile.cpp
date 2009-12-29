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

Projectile::Projectile(Object* creator) : Object(creator, ObjectType_Projectile)
{

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

	// Create a particle effect
	ParticleSystem* explosion = new ParticleSystem((ImageResource*)ResourceManager::Get()->GetResource("particle_explosion"), 0.5f);
	explosion->SetPosition(bounds.center);
	Game::Get()->GetFXManager()->RegisterEffect(explosion);

	// Simulate the explosion on the world
	Game::Get()->GetWorld()->SimulateExplosion(bounds.center, explosionData);

}

float Projectile::GetTimer() const
{

	return timer;

}

void Projectile::SetTimer(float newTime)
{

	timer = newTime;

}

const ExplosionData& Projectile::GetExplosionData() const
{

	return explosionData;

}

void Projectile::SetExplosionData(const ExplosionData& data)
{

	explosionData = data;

}

/*
	class Projectile_Bazooka
*/

Projectile_Bazooka::Projectile_Bazooka(Object* creator) : Projectile(creator)
{

	timer = -1.0f;

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

Projectile_Mine::Projectile_Mine(float armTime, float dudChance) : Projectile(NULL)
{

	timer = -1.0f;
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
	else
	{

		//
		// Check for nearby slugs
		//

		std::vector<Object*> objects;

		const float DETONATION_RADIUS = 30.0f;
		Game::Get()->GetWorld()->GetObjectsNear(objects, this, DETONATION_RADIUS, ObjectType_Slug);

		// Explode if we found a slug in the detonation radius
		if (objects.size() > 0)
			Die();

	}

	return Projectile::Update(elapsedTime, gravity, wind);

}

void Projectile_Mine::DebugRender()
{

	// Show detonation radius
	Renderer::Get()->DrawDebugCircle(GetPosition(), 30.0f, Color::red);

}

/*
	class Projectile_HomingMissile
*/

Projectile_HomingMissile::Projectile_HomingMissile(Object* creator, const Vec2f& targetPosition) : Projectile_Bazooka(creator)
{

	target = targetPosition;
	timer = 10.0f;
	armTimer = 0.5f;

}

bool Projectile_HomingMissile::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	// Adjust velocity to try to match the direction of the target
	const float ADJUSTMENT_SPEED = 100.0f;

	// Start homing as soon as the arm timer runs out
	if (armTimer <= 0.0f)
	{

		Vec2f directionToTarget = (target - GetPosition()).Normalize();
		velocity += directionToTarget * ADJUSTMENT_SPEED;

	}
	else
		armTimer -= elapsedTime;

	return Projectile_Bazooka::Update(elapsedTime, gravity, wind);

}

/*
	class Projectile_Dynamite
*/

Projectile_Dynamite::Projectile_Dynamite() : Projectile(NULL)
{

	timer = 3.0f;

}

bool Projectile_Dynamite::OnCollideWithTerrain()
{

	return Object::OnCollideWithTerrain();

}

void Projectile_Dynamite::OnCollideWithObject(Object* object)
{

	// Do nothing

}