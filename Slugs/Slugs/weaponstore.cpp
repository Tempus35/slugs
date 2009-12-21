#include "weaponstore.h"

/*
	class WeaponStore
*/

WeaponStore::WeaponStore()
{

}

WeaponStore::~WeaponStore()
{

	for (unsigned int i = 0; i < weapons.size(); ++ i)
		SafeDelete(weapons[i]);

}

void WeaponStore::Add(Weapon* weapon)
{

	ASSERT(weapon != NULL);

	if (!Get(weapon->GetType()))
		weapons.push_back(weapon);
	else
		SafeDelete(weapon);

}

Weapon* WeaponStore::Get()
{

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