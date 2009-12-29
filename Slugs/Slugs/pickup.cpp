#include "pickup.h"
#include "game.h"

/*
	class Pickup
*/

Pickup::Pickup() : Object(NULL, ObjectType_Pickup)
{

}

void Pickup::OnCollideWithObject(Object* object)
{

	// Die if picked up
	if (object->GetType() == ObjectType_Slug)
		Die();

}

/*
	class Pickup_Exploding
*/

void Pickup_Exploding::Die(bool instant)
{

	if (!instant)
	{

		alive = false;
		Explode();

	}

}

void Pickup_Exploding::Explode()
{

	const ExplosionData explosionData(50.0f, 85.0f, 400.0f, 50.0f, 40.0f);

	Game::Get()->GetWorld()->SimulateExplosion(GetPosition(), explosionData);

}

/*
	class Pickup_Health
*/

void Pickup_Health::OnCollideWithObject(Object* object)
{

	if (object->GetType() == ObjectType_Slug)
	{

		Die();

		//
		// Add the hitpoints to the slug, but don't increase over the starting hitpoints
		//

		int bonus = Game::Get()->GetGameInt(GameInt_HealthPickupAmount);
		int maxHitpoints = Game::Get()->GetGameInt(GameInt_SlugHitpoints);

		Slug* slug = (Slug*)object;
		slug->SetHitpoints(Min(slug->GetHitPoints() + bonus, maxHitpoints));

	}

}

/*
	class Pickup_Weapon
*/

Pickup_Weapon::Pickup_Weapon()
{

	// TODO: Randomize contents
	type = WeaponType_Bazooka;
	ammo = 1;

}

Pickup_Weapon::Pickup_Weapon(WeaponType weaponType, int ammoCount)
{

	type = weaponType;
	ammo = ammoCount;

}

void Pickup_Weapon::OnCollideWithObject(Object* object)
{

	if (object->GetType() == ObjectType_Slug)
	{

		Die();

		Slug* slug = (Slug*)object;
		slug->GiveWeapon(type, ammo);

	}

}