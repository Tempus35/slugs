//---------------------------------------------------------------
//
// Slugs
// weapon.cpp
//
//---------------------------------------------------------------

#include "weapon.h"
#include "game.h"
#include "slug.h"
#include "world.h"

/*
	class Weapon
*/

Weapon::Weapon(WeaponType t, int initialAmmo, bool charges)
{

	type = t;
	ammo = initialAmmo;
	needsCharge = charges;

}

Weapon* Weapon::CreateFromType(WeaponType t)
{

	switch (t)
	{

	case WeaponType_Bazooka:
		return new Weapon_Bazooka();

	case WeaponType_Grenade:
		return new Weapon_Grenade();

	case WeaponType_Shotgun:
		return new Weapon_Shotgun();

	case WeaponType_Machinegun:
		return new Weapon_Machinegun();

	case WeaponType_Mine:
		return new Weapon_Mine();

	}

	ASSERTMSG(0, "Weapon::CreateFromType - Invalid WeaponType!");

	return NULL;

}

WeaponType Weapon::GetType() const
{

	return type;

}

int Weapon::GetAmmo() const
{

	return ammo;

}

bool Weapon::NeedsCharge() const
{

	return needsCharge;

}

bool Weapon::TakeAmmo()
{

	switch (ammo)
	{

	case -1:
		return true;

	case 0:
		return false;

	default:

		ASSERT(ammo > 0);
		ammo --;

		return true;

	}

}

/*
	class Weapon_Bazooka
*/

Weapon_Bazooka::Weapon_Bazooka(int initialAmmo) : Weapon(WeaponType_Bazooka, initialAmmo, true)
{

}

bool Weapon_Bazooka::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		//
		// Okay to fire
		//

		float aimAngle = owner->GetAimAngle();
		Vec2f aimDirection = Vec2f(Cos(aimAngle), Sin(aimAngle));

		if (owner->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			aimDirection.x = -aimDirection.x;

		float shotVelocity = owner->GetPower() * 1500.0f;
		Vec2f aimVelocity = aimDirection * shotVelocity;

		Projectile_Bazooka* projectile = new Projectile_Bazooka(owner);
		projectile->SetPosition(owner->GetWeaponPoint());
		projectile->SetTimer(-1);
		projectile->SetBounds(5.0f, 5.0f);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_rocket")));

		const ExplosionData explosionData(75.0f, 85.0f, 400.0f, 75.0f, 50.0f);
		projectile->SetExplosionData(explosionData);

		projectile->SetVelocity(aimVelocity);

		// Add the projectile to the world
		Game::Get()->GetWorld()->AddCreatedObject(projectile);

		projectileCreated = projectile;

		return true;

	}

	return false;

}

/*
	Weapon_Grenade
*/

Weapon_Grenade::Weapon_Grenade(int initialAmmo) : Weapon(WeaponType_Grenade, initialAmmo, true)
{

}

bool Weapon_Grenade::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		//
		// Okay to fire
		//

		float aimAngle = owner->GetAimAngle();
		Vec2f aimDirection = Vec2f(Cos(aimAngle), Sin(aimAngle));

		if (owner->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			aimDirection.x = -aimDirection.x;

		float shotVelocity = owner->GetPower() * 1500.0f;
		Vec2f aimVelocity = aimDirection * shotVelocity;

		Projectile_Grenade* projectile = new Projectile_Grenade(owner);
		projectile->SetPosition(owner->GetWeaponPoint());
		projectile->SetTimer(3);
		projectile->SetBounds(5.0f, 5.0f);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_grenade")));

		const ExplosionData explosionData(75.0f, 85.0f, 400.0f, 75.0f, 50.0f);
		projectile->SetExplosionData(explosionData);

		projectile->SetVelocity(aimVelocity);

		// Add the projectile to the world
		Game::Get()->GetWorld()->AddCreatedObject(projectile);

		projectileCreated = projectile;

		return true;

	}

	return false;

}

/*
	Weapon_Shotgun
*/

Weapon_Shotgun::Weapon_Shotgun(int initialAmmo) : Weapon(WeaponType_Shotgun, initialAmmo, false)
{

}

bool Weapon_Shotgun::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		//
		// Fire 5 bullets in a spread pattern
		//

		const ExplosionData explosionData(10.0f, 15.0f, 25.0f, 10.0f, 20.0f);

		const float spread = Radians(10.0f);
		float baseAngle = owner->GetAimAngle();

		for (int i = 0; i < 5; ++ i)
		{

			float angle = baseAngle + Random::RandomFloat(-spread, spread);
			Vec2f direction = Vec2f(Cos(angle), Sin(angle));

			if (owner->GetFacingDirection() != FACINGDIRECTION_RIGHT)
				direction.x = -direction.x;

			Intersection intersection = Game::Get()->GetWorld()->GetRayIntersection(owner->GetPosition(), direction, owner);	

			if (intersection.type != IntersectionType_None)
				Game::Get()->GetWorld()->DeferExplosion(intersection.position, explosionData);

		}

		Game::Get()->GetWorld()->SimulateExplosions();

		return true;

	}

	return false;

}

/*
	Weapon_Machinegun
*/

Weapon_Machinegun::Weapon_Machinegun(int initialAmmo) : Weapon(WeaponType_Machinegun, initialAmmo, false)
{

	firing = false;
	fireTimer = 0.0f;
	fireCounter = 0;

}

bool Weapon_Machinegun::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	// Store the owner, we need it inside the update method
	slug = owner;

	if (TakeAmmo() == true)
	{

		//
		// Set the weapon up to fire 5 shots
		// This will occur during the update method
		//

		const int numShots = 5;
		
		firing = true;
		fireCounter = numShots;
		fireTimer = 0.0f;

		// Register for updates so we can fire over time
		Game::Get()->GetUpdateManager()->RegisterForUpdates(this);

		return true;

	}

	return false;

}

void Weapon_Machinegun::Update(float elapsedTime)
{

	const float timeBetweenShots = 0.1f;				// Time between shots in seconds
	const float upSwing = Radians(2.0f);				// After each shot fires we adjust the slugs aim slightly

	fireTimer -= elapsedTime;
	
	if (fireTimer <= 0.0f)
	{

		//
		// Make boom
		//

		const ExplosionData explosionData(10.0f, 15.0f, 25.0f, 10.0f, 20.0f);

		float angle = slug->GetAimAngle();
		
		Vec2f direction = Vec2f(Cos(angle), Sin(angle));

		if (slug->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			direction.x = -direction.x;

		Intersection intersection = Game::Get()->GetWorld()->GetRayIntersection(slug->GetPosition(), direction, slug);	

		if (intersection.type != IntersectionType_None)
			Game::Get()->GetWorld()->SimulateExplosion(intersection.position, explosionData);

		// Force slug to aim up a little
		slug->AdjustAim(upSwing);

		fireCounter --;

		if (fireCounter <= 0)
			Game::Get()->GetUpdateManager()->UnregisterForUpdates(this);
		else
			fireTimer = timeBetweenShots;

	}

}

/*
	class Weapon_Mine
*/

Weapon_Mine::Weapon_Mine(int initialAmmo) : Weapon(WeaponType_Mine, initialAmmo, false)
{

}

bool Weapon_Mine::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		const float armTime = 2.0f;
		const float dudChance = 0.05f;

		Projectile_Mine* projectile = new Projectile_Mine(NULL, armTime, dudChance);
		projectile->SetPosition(owner->GetWeaponPoint());
		projectile->SetTimer(-1.0f);
		projectile->SetBounds(5.0f, 5.0f);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_mine")));
		projectile->SetVelocity(Vec2f(0.0f, -200.0f));

		const ExplosionData explosionData(50.0f, 60.0f, 300.0f, 50.0f, 40.0f);
		projectile->SetExplosionData(explosionData);

		// Add the projectile to the world
		Game::Get()->GetWorld()->AddCreatedObject(projectile);

		return true;

	}

	return false;

}