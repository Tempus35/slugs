//---------------------------------------------------------------
//
// Slugs
// slug.cpp
//
//---------------------------------------------------------------

#include "slug.h"

#include "game.h"
#include "team.h"
#include "resourcemanager.h"
#include "world.h"
#include "terrain.h"
#include "aicontroller.h"

Slug::Slug(Team* _team, AIController* aiController) : Object(NULL, ObjectType_Slug)
{

	const float SLUG_DEATH_TIMER = 2.0f;			// Numbers of seconds before a slug explodes after reaching 0 hps

	controller = aiController;
	team = _team;

	movementDirection = MOVEMENTDIRECTION_NONE;
	facingDirection = FACINGDIRECTION_RIGHT;
	aimAngle = 0.0f;

	power = 0.0f;
	charging = false;
	currentWeapon = NULL;
	weaponStore = NULL;

	hps = 100;
	death = SLUG_DEATH_TIMER;

	ImageResource* r = (ImageResource*)ResourceManager::Get()->GetResource("image_slug_right");
	SetImage(r);
	SetBounds(8.0f, 8.0f);
	SetWeapons(new WeaponStore(true), true);
	ArmSelf();

}

Slug::~Slug()
{

	if (ownsWeaponStore)
		SafeDelete(weaponStore);

}

bool Slug::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	const float SLUG_MAX_UP_STEP = 5.0f;			// Maximum number of pixels a slug can move up per pixel moved left/right
	const float SLUG_MAX_DOWN_STEP = 10.0f;			// Maximum number of pixels a slug can move down without falling
	const float SLUG_MOVEMENT_SPEED = 50.0f;		// Slug movement speed in pixels/second
	const float SLUG_AIM_SPEED = Radians(45.0f);	// Rate at which the slug can aim in radians/second
	const float SHOT_POWER_CHARGE_RATE = 1.0f;		// Rate of weapon charge in units/second	

	//
	// Find out if we are alive
	//

	bool moved = false;

	if (hps > 0)
	{

		// Update our ai controller if we have one
		if (controller)
			controller->Update(this, elapsedTime);

		//
		// Update shot power if slug is charging a weapon
		//

		if (charging)
		{

			power += elapsedTime * SHOT_POWER_CHARGE_RATE;

			// Fire when we reach max power
			if (power >= 1.0f)
				Fire();
				
		}
		
		//
		// Movement - only works when in the resting state
		//

		if (atRest)
		{

			float direction = 0.0f;

			if (movementDirection & MOVEMENTDIRECTION_RIGHT)
			{
				direction = 1.0f;
				SetImage((ImageResource*)ResourceManager::Get()->GetResource("image_slug_right"));
			}
			else if (movementDirection & MOVEMENTDIRECTION_LEFT)
			{
				direction = -1.0f;
				SetImage((ImageResource*)ResourceManager::Get()->GetResource("image_slug_left"));
			}

			if (direction != 0.0f)
			{

				float height = Game::Get()->GetWorld()->GetTerrain()->GetHeightAt(Vec2f(bounds.center.x + direction, bounds.center.y));
		
				float dy = height - (bounds.center.y - bounds.extents.y);

				if ((dy >= 0.0f) && (dy <= SLUG_MAX_UP_STEP))
				{
				
					moved = true;
					bounds.center.x += direction * elapsedTime * SLUG_MOVEMENT_SPEED;
					bounds.center.y = height + bounds.extents.y + 1.0f;
				
				}
				else if (dy < 0.0f)
				{

					if (Abs(dy) <= SLUG_MAX_DOWN_STEP)
					{

						moved = true;
						bounds.center.x += direction * elapsedTime * SLUG_MOVEMENT_SPEED;
						bounds.center.y = height + bounds.extents.y + 1.0f;

					}
					else
					{

						moved = true;
						bounds.center.x += Max(elapsedTime * SLUG_MOVEMENT_SPEED, 1.0f) * direction;
						SetAtRest(false);

						// Throw the slug off the edge
						SetVelocity(Vec2f(direction * 25.0f, 5.0f));

					}

				}

			}

		}

		//
		// Update aim
		//

		if (movementDirection & MOVEMENTDIRECTION_UP)
		{

			aimAngle += elapsedTime * SLUG_AIM_SPEED;

			if (aimAngle > Math::PI_OVER_2)
				aimAngle = Math::PI_OVER_2;

		}

		if (movementDirection & MOVEMENTDIRECTION_DOWN)
		{

			aimAngle -= elapsedTime * SLUG_AIM_SPEED;

			if (aimAngle < -Math::PI_OVER_2)
				aimAngle = -Math::PI_OVER_2;

		}

		if (moved)
		{

			// Update our sprite since we moved
			Moved();
			
		}

	}
	else
	{

		//
		// Slug is dying
		// 

		death -= elapsedTime;

		if (death <= 0.0f)
		{

			alive = false;

			// Go boom when our timer runs out
			Explode();

			// Create a gravestone
			SpawnGravestone();

		}

	}

	// Physics
	return Object::Update(elapsedTime, gravity, wind);

}

void Slug::StartMovingLeft()
{

	movementDirection |= MOVEMENTDIRECTION_LEFT;
	facingDirection = FACINGDIRECTION_LEFT;

}	

void Slug::StartMovingRight()
{

	movementDirection |= MOVEMENTDIRECTION_RIGHT;
	facingDirection = FACINGDIRECTION_RIGHT;

}

void Slug::StopMoving()
{

	movementDirection &= ~(MOVEMENTDIRECTION_LEFT|MOVEMENTDIRECTION_RIGHT);
	
}

void Slug::StartAimingUp()
{

	movementDirection |= MOVEMENTDIRECTION_UP;

}

void Slug::StartAimingDown()
{

	movementDirection |= MOVEMENTDIRECTION_DOWN;

}

void Slug::StopAiming()
{

	movementDirection &= ~(MOVEMENTDIRECTION_UP|MOVEMENTDIRECTION_DOWN);

}

void Slug::StartCharging()
{

	if (!charging)
	{

		power = 0.0f;
		charging = true;

	}

}

void Slug::Jump()
{

	//
	// We can only jump if we are in the resting state
	// Cancel the resting state and give the slug a velocity in the correct direction
	//

	if (atRest)
	{
	
		SetAtRest(false);

		if (facingDirection == FACINGDIRECTION_LEFT)
			velocity.x = -100.0f;
		else
			velocity.x = 100.0f;

		velocity.y = 200.0f;

	}

}

void Slug::Fire()
{

	if (charging)
	{

		//
		// Fire the current weapon
		//

		bool fired = false;

		if (currentWeapon)
			fired = currentWeapon->Fire(this);

		if (!fired)
		{

			// TODO: Play click sound

		}

		// We are no longer charging the weapon
		charging = false;

	}

}

FaceDirection Slug::GetFacingDirection() const
{

	return facingDirection;

}

float Slug::GetAimAngle() const
{

	return aimAngle;

}

void Slug::AdjustAim(float amount)
{

	aimAngle += amount;

	if (aimAngle >= Math::PI_OVER_2)
		aimAngle = Math::PI_OVER_2;

}

float Slug::GetPower() const
{

	return power;

}

void Slug::Die(bool instant)
{

	if (instant)
		alive = false;

}

void Slug::Explode()
{

	const float SLUG_EXPLOSION_STRENGTH = 30.0f;	// Strength of the death explosion

	Game::Get()->GetWorld()->SimulateExplosion(bounds.center, SLUG_EXPLOSION_STRENGTH);

}

void Slug::SpawnGravestone() const
{

	// TODO: Get image resource based on team gravestone style
	Gravestone* gravestone = new Gravestone((ImageResource*)ResourceManager::Get()->GetResource("image_gravestone"));

	if (gravestone)
	{

		// Set to the current position
		gravestone->SetPosition(bounds.center);
		gravestone->SetBounds(8.0f, 8.0f);

		// Add the gravestone to the world
		Game::Get()->GetWorld()->AddCreatedObject(gravestone);

	}

}

void Slug::ArmSelf()
{

	if ((!currentWeapon) && (weaponStore))
		currentWeapon = weaponStore->Get();

}

void Slug::ArmSelf(WeaponType type)
{

	currentWeapon = weaponStore->Get(type);

}

void Slug::SetWeapons(WeaponStore* store, bool slugOwns)
{

	ownsWeaponStore = slugOwns;
	weaponStore = store;

	if (currentWeapon)
	{

		currentWeapon = NULL;
		ArmSelf();

	}

}

Vec2f Slug::GetWeaponPoint() const
{

	Vec2f aimDirection = Vec2f(Cos(aimAngle), Sin(aimAngle));

	if (facingDirection != FACINGDIRECTION_RIGHT)
		aimDirection.x = -aimDirection.x;

	return bounds.center + aimDirection * bounds.extents.x;

}

Team* Slug::GetTeam() const
{

	return team;

}