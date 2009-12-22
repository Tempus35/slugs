//---------------------------------------------------------------
//
// Slugs
// weapon.cpp
//
//---------------------------------------------------------------

#include "weapon.h"

#include "slug.h"

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

	case WeaponType_MachineGun:
		return new Weapon_MachineGun();

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

bool Weapon_Bazooka::Fire(Slug* owner)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		//
		// Okay to fire
		//

		float aimAngle = owner->GetAimAngle();
		Vector2 aimDirection = Vector2(cosf(aimAngle), sinf(aimAngle));

		if (owner->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			aimDirection.x = -aimDirection.x;

		float shotVelocity = owner->GetPower() * 1000.0f;
		Vector2 aimVelocity = aimDirection * shotVelocity;

		Projectile_Bazooka* projectile = new Projectile_Bazooka(owner);
		projectile->SetPosition(owner->GetPosition() + aimDirection * (float)owner->GetRadius());
		projectile->SetStrength(75);
		projectile->SetTimer(-1);
		projectile->SetRadius(5);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_rocket")));

		projectile->SetVelocity(aimVelocity);

		// Add the projectile to the world
		World::Get()->AddCreatedObject(projectile);

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

bool Weapon_Grenade::Fire(Slug* owner)
{

	return false;

}

/*
	Weapon_Shotgun
*/

Weapon_Shotgun::Weapon_Shotgun(int initialAmmo) : Weapon(WeaponType_Shotgun, initialAmmo, false)
{

}

bool Weapon_Shotgun::Fire(Slug* owner)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		//
		// Fire 5 bullets in a spread pattern
		//

		const float spread = Radians(10.0f);
		const int strength = 10;
		float baseAngle = owner->GetAimAngle();

		for (int i = 0; i < 5; ++ i)
		{

			float angle = baseAngle + Random::RandomFloat(-spread, spread);
			Vector2 direction = Vector2(Cos(angle), Sin(angle));

			if (owner->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			direction.x = -direction.x;

			Vector2 collisionPos;
			bool collision =  World::Get()->GetRayIntersection(owner->GetPosition(), direction, collisionPos, owner);	

			if (collision)
				World::Get()->DeferExplosion((int)collisionPos.x, (int)collisionPos.y, strength);

		}

		World::Get()->SimulateExplosions();

		return true;

	}

	return false;

}

/*
	Weapon_MachineGun
*/

Weapon_MachineGun::Weapon_MachineGun(int initialAmmo) : Weapon(WeaponType_MachineGun, initialAmmo, false)
{

	firing = false;
	fireTimer = 0.0f;
	fireCounter = 0;

}

bool Weapon_MachineGun::Fire(Slug* owner)
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
		UpdateManager::Get()->RegisterForUpdates(this);

	}

	return false;

}

void Weapon_MachineGun::Update(float elapsedTime)
{

	const float timeBetweenShots = 0.1f;				// Time between shots in seconds
	const float upSwing = Radians(2.0f);				// After each shot fires we adjust the slugs aim slightly

	fireTimer -= elapsedTime;
	
	if (fireTimer <= 0.0f)
	{

		//
		// Make boom
		//

		const int strength = 10;
		float angle = slug->GetAimAngle();

		Vector2 direction = Vector2(Cos(angle), Sin(angle));

		if (slug->GetFacingDirection() != FACINGDIRECTION_RIGHT)
			direction.x = -direction.x;

		Vector2 collisionPos;
		bool collision =  World::Get()->GetRayIntersection(slug->GetPosition(), direction, collisionPos, slug);	

		if (collision)
			World::Get()->SimulateExplosion((int)collisionPos.x, (int)collisionPos.y, strength);

		// Force slug to aim up a little
		slug->AdjustAim(upSwing);

		fireCounter --;

		if (fireCounter <= 0)
			UpdateManager::Get()->UnregisterForUpdates(this);
		else
			fireTimer = timeBetweenShots;

	}

}