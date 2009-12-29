//---------------------------------------------------------------
//
// Slugs
// weaponstore.cpp
//
//---------------------------------------------------------------

#include "weaponstore.h"

/*
	class WeaponStore
*/

WeaponStore::WeaponStore(bool debug)
{

	if (debug)
	{

		for (int i = 0; i < WeaponType_LAST; ++ i)
			Add(Weapon::CreateFromType((WeaponType)i));

	}

}

WeaponStore::~WeaponStore()
{

	// Free weapons
	for (unsigned int i = 0; i < weapons.size(); ++ i)
		SafeDelete(weapons[i]);

}

void WeaponStore::Add(Weapon* weapon)
{

	ASSERT(weapon != NULL);

	//
	// Add the weapon to the store if we don't already have one of its type
	//

	if (!Get(weapon->GetType()))
		weapons.push_back(weapon);
	else
		SafeDelete(weapon);

}

void WeaponStore::Add(WeaponType weaponType, int ammo)
{

	// Get a pointer to the weapon if we have it
	Weapon* weapon = Get(weaponType);

	// If we don't have the weapon then add it with either zero of infinite ammo
	if (!weapon)
	{

		weapon = Weapon::CreateFromType(weaponType, (ammo == -1) ? -1 : 0);
		Add(weapon);

	}

	// Add the ammo to the weapon
	weapon->AddAmmo(ammo);

}

Weapon* WeaponStore::Get() const
{

	//
	// Find the first weapon with ammo
	//

	for (unsigned int i = 0; i < weapons.size(); ++ i)
	{

		int ammo = weapons[i]->GetAmmo();

		if ((ammo == -1) || (ammo > 0))
			return weapons[i];

	}

	return NULL;

}

Weapon* WeaponStore::Get(WeaponType type, bool getEmptyWeapons) const
{

	//
	// Find a weapon matching the requested type
	//

	for (unsigned int i = 0; i < weapons.size(); ++ i)
	{

		if (weapons[i]->GetType() == type)
		{

			if ((getEmptyWeapons) || (weapons[i]->GetAmmo() > 0))
				return weapons[i];

		}

	}

	return NULL;

}