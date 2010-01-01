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

Weapon::Weapon(WeaponType t, int initialAmmo, bool charges, bool aims)
{

	type = t;
	ammo = initialAmmo;
	needsCharge = charges;
	requiresAiming = aims;

}

Weapon* Weapon::CreateFromType(WeaponType t, int ammo)
{

	switch (t)
	{

	case WeaponType_Bazooka:
		return new Weapon_Bazooka(ammo);

	case WeaponType_Grenade:
		return new Weapon_Grenade(ammo);

	case WeaponType_Shotgun:
		return new Weapon_Shotgun(ammo);

	case WeaponType_Machinegun:
		return new Weapon_Machinegun(ammo);

	case WeaponType_Mine:
		return new Weapon_Mine(ammo);

	case WeaponType_HomingMissile:
		return new Weapon_HomingMissile(ammo);

	case WeaponType_Teleporter:
		return new Weapon_Teleporter(ammo);

	case WeaponType_Airstrike:
		return new Weapon_Airstrike(ammo);

	case WeaponType_Dynamite:
		return new Weapon_Dynamite(ammo);

	case WeaponType_Clusterbomb:
		return new Weapon_Clusterbomb(ammo);

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

void Weapon::AddAmmo(int amount)
{

	// Only add the ammo for weapons without infinite ammo
	if (ammo != -1)
		ammo += amount;

}

bool Weapon::IsTargetable() const
{

	return false;

}

float Weapon::GetLaunchSpeed(float power) const
{

	return 0.0f;

}

void Weapon::DebugRender()
{

}

bool Weapon::RequiresAiming() const
{

	return requiresAiming;

}

/*
	class TargetableWeapon
*/

TargetableWeapon::TargetableWeapon(WeaponType t, int initialAmmo, bool charges, bool aims) : Weapon(t, initialAmmo, charges, aims)
{

	targetSelected = false;

}

void TargetableWeapon::SetTargetPoint(const Vec2f& point)
{

	targetSelected = true;
	targetPoint = point;

}

bool TargetableWeapon::IsTargetable() const
{

	return true;

}

bool TargetableWeapon::HasTarget() const
{

	return targetSelected;

}

const Vec2f& TargetableWeapon::GetTarget() const
{

	return targetPoint;

}

/*
	class Weapon_Bazooka
*/

Weapon_Bazooka::Weapon_Bazooka(int initialAmmo) : Weapon(WeaponType_Bazooka, initialAmmo, true, true)
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

		Vec2f aimDirection = owner->GetAimDirection();

		float shotVelocity = GetLaunchSpeed(owner->GetPower());
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

float Weapon_Bazooka::GetLaunchSpeed(float power) const
{

	ASSERT((power >= 0.0f) && (power <= 1.0f));

	return power * 1500.0f;

}


/*
	Weapon_Grenade
*/

Weapon_Grenade::Weapon_Grenade(int initialAmmo) : Weapon(WeaponType_Grenade, initialAmmo, true, true)
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

		Vec2f aimDirection = owner->GetAimDirection();

		float shotVelocity = GetLaunchSpeed(owner->GetPower());
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

float Weapon_Grenade::GetLaunchSpeed(float power) const
{

	return power * 1500.0f;

}

/*
	Weapon_Shotgun
*/

Weapon_Shotgun::Weapon_Shotgun(int initialAmmo) : Weapon(WeaponType_Shotgun, initialAmmo, false, true)
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

		const ExplosionData explosionData(10.0f, 10.0f, 200.0f, 10.0f, 10.0f, true);

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

Weapon_Machinegun::Weapon_Machinegun(int initialAmmo) : Weapon(WeaponType_Machinegun, initialAmmo, false, true)
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

		const ExplosionData explosionData(10.0f, 10.0f, 200.0f, 10.0f, 10.0f, true);

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

Weapon_Mine::Weapon_Mine(int initialAmmo) : Weapon(WeaponType_Mine, initialAmmo, false, false)
{

}

bool Weapon_Mine::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		const float armTime = 2.0f;
		const float dudChance = 0.05f;

		Projectile_Mine* projectile = new Projectile_Mine(armTime, dudChance);
		projectile->SetPosition(owner->GetWeaponPoint());

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

/*
	class Weapon_HomingMissile
*/

Weapon_HomingMissile::Weapon_HomingMissile(int initialAmmo) : TargetableWeapon(WeaponType_HomingMissile, initialAmmo, true, true)
{

}

bool Weapon_HomingMissile::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	// Requires a target set
	if (!targetSelected)
		return false;

	if (TakeAmmo() == true)
	{

		//
		// Okay to fire
		//

		Vec2f aimDirection = owner->GetAimDirection();

		float shotVelocity = GetLaunchSpeed(owner->GetPower());
		Vec2f aimVelocity = aimDirection * shotVelocity;

		Projectile_HomingMissile* projectile = new Projectile_HomingMissile(owner, targetPoint);
		projectile->SetPosition(owner->GetWeaponPoint());
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

float Weapon_HomingMissile::GetLaunchSpeed(float power) const
{

	return power * 1500.0f;

}

/*
	class Weapon_Teleporter
*/

Weapon_Teleporter::Weapon_Teleporter(int initialAmmo) : TargetableWeapon(WeaponType_Teleporter, initialAmmo, false, false)
{

}

bool Weapon_Teleporter::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	// Requires a target set
	if (!targetSelected)
		return false;

	if (TakeAmmo() == true)
	{

		owner->SetPosition(targetPoint);

		// Clear the target
		targetSelected = false;

		return true;

	}

	return false;

}

void Weapon_Teleporter::SetTargetPoint(const Vec2f& point)
{

	const float REQUIRED_RADIUS = 50.0f;

	// Ensure the area around the point is clear
	if (!Game::Get()->GetWorld()->GetCircleIntersection(point, REQUIRED_RADIUS))
	{

		targetSelected = true;
		targetPoint = point;

	}

}

/*
	class Weapon_Airstrike
*/

Weapon_Airstrike::Weapon_Airstrike(int initialAmmo) : TargetableWeapon(WeaponType_Airstrike, initialAmmo, false, false)
{

}

bool Weapon_Airstrike::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	// Requires a target set
	if (!targetSelected)
		return false;

	if (TakeAmmo() == true)
	{

		const float ANGLE_OF_ATTACK		= Radians(45.0f);
		const float SEPARATION			= 50.0f;
		const int   PROJECTILES			= 5;
		const float PROJECTILE_SPEED	= GetLaunchSpeed();

		// Calculate direction of the attack based on x position in the world
		float direction = targetPoint.x > Game::Get()->GetWorld()->WidthInPixels() / 2 ? 1.0f : -1.0f;

		// Calculate launch height
		float sy = Game::Get()->GetWorld()->HeightInPixels() + 50.0f;

		// Calculate difference in height from target and drop
		float dy = sy - targetPoint.y;

		// Calculate x offset to the launch position based on angle of attack
		float offset = Tan(ANGLE_OF_ATTACK) * dy;

		// Calculate start point for the projectiles
		Vec2f start = Vec2f(targetPoint.x - offset * direction, sy);
		launchPoint = start;

		// Calculate direction for projectiles
		Vec2f projectileDirection;

		bool canHit = CalculateLaunchDirection(start, targetPoint, PROJECTILE_SPEED, Game::Get()->GetWorld()->Gravity().Length(), projectileDirection);
		ASSERTMSG(canHit, "If this is hit we need to increase the launch speed of the airstrike.");

		launchDirection = projectileDirection;

		Vec2f perpendicular = projectileDirection.Perpendicular();

		// Adjust start point for the first projectile
		start -= perpendicular * (direction * SEPARATION * (float)PROJECTILES * 0.5f);

		//
		// Launch the projectiles
		//

		for (int i = 0; i < PROJECTILES; ++ i)
		{

			Projectile_Bazooka* projectile = new Projectile_Bazooka(NULL);
			projectile->SetPosition(start);
			projectile->SetBounds(5.0f, 5.0f);
			projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_rocket")));
			projectile->SetHitpoints(1000);
			//projectile->SetAffectedByGravity(false);
			projectile->SetForceImmune(true);

			const ExplosionData explosionData(50.0f, 85.0f, 400.0f, 50.0f, 40.0f);
			projectile->SetExplosionData(explosionData);

			projectile->SetVelocity(projectileDirection * PROJECTILE_SPEED);

			// Add the projectile to the world
			Game::Get()->GetWorld()->AddCreatedObject(projectile);

			// Adjust offset
			start += perpendicular * direction * Random::RandomFloat(0.75f * SEPARATION, 1.25f * SEPARATION) + projectileDirection * Random::RandomFloat(-SEPARATION * 0.25f, SEPARATION * 0.25f);

		}

		return true;

	}

	return false;

}

float Weapon_Airstrike::GetLaunchSpeed(float power) const
{

	return 1000.0f;

}

void Weapon_Airstrike::DebugRender()
{

	Renderer::Get()->DrawDebugTrajectory(launchPoint, launchDirection, GetLaunchSpeed(), Game::Get()->GetWorld()->Gravity().Length(), Color::purple);

}


/*
	class Weapon_Dynamite
*/

Weapon_Dynamite::Weapon_Dynamite(int initialAmmo) : Weapon(WeaponType_Dynamite, initialAmmo, false, false)
{

}

bool Weapon_Dynamite::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		Projectile_Dynamite* projectile = new Projectile_Dynamite();
		projectile->SetPosition(owner->GetWeaponPoint());
		projectile->SetBounds(5.0f, 5.0f);
		projectile->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_dynamite")));
		projectile->SetVelocity(Vec2f(0.0f, -200.0f));

		const ExplosionData explosionData(120.0f, 150.0f, 600.0f, 150.0f,75.0f);
		projectile->SetExplosionData(explosionData);

		// Add the projectile to the world
		Game::Get()->GetWorld()->AddCreatedObject(projectile);

		return true;

	}

	return false;

}

/*
	class Weapon_Clusterbomb
*/

Weapon_Clusterbomb::Weapon_Clusterbomb(int initialAmmo) : Weapon(WeaponType_Clusterbomb, initialAmmo, true, true)
{

}

bool Weapon_Clusterbomb::Fire(Slug* owner, Projectile*& projectileCreated)
{

	ASSERT(owner);

	if (TakeAmmo() == true)
	{

		Vec2f aimDirection = owner->GetAimDirection();

		float shotVelocity = GetLaunchSpeed(owner->GetPower());
		Vec2f aimVelocity = aimDirection * shotVelocity;

		Projectile_Clusterbomb* projectile = new Projectile_Clusterbomb(owner);
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

float Weapon_Clusterbomb::GetLaunchSpeed(float power) const
{

	return 1500.0f * power;

}