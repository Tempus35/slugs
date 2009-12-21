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

		Projectile_Bazooka* projectile = new Projectile_Bazooka(owner);
		projectile->SetPosition(owner->GetPosition());
		projectile->SetStrength(75);
		projectile->SetTimer(-1);
		projectile->SetRadius(5);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_gravestone")));

		float shotVelocity = owner->GetPower() * SHOT_POWER_MULTIPLIER;
		float cosAnglePower = cosf(owner->GetAimAngle()) * shotVelocity;
		float sinAnglePower = sinf(owner->GetAimAngle()) * shotVelocity;

		if (owner->GetFacingDirection() == FACINGDIRECTION_RIGHT)
			projectile->SetVelocity(cosAnglePower, sinAnglePower);
		else
			projectile->SetVelocity(-cosAnglePower, sinAnglePower);

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

	return false;

}

/*
	Weapon_MachineGun
*/

Weapon_MachineGun::Weapon_MachineGun(int initialAmmo) : Weapon(WeaponType_MachineGun, initialAmmo, false)
{

}

bool Weapon_MachineGun::Fire(Slug* owner)
{

	return false;

}