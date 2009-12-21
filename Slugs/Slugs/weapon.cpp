#include "weapon.h"

/*
	class Weapon
*/

Weapon::Weapon(WeaponType t, int initialAmmo, bool charges)
{

	type = t;
	ammo = initialAmmo;
	needsCharge = charges;

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

bool Weapon_Bazooka::Fire()
{

	if (TakeAmmo() == true)
	{

		//
		// Okay to fire
		//

		return true;

	}

	return false;

}