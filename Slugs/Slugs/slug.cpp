#include "slug.h"

#include "team.h"
#include "resourcemanager.h"

Slug::Slug() : Object(ObjectType_Slug)
{

	movementDirection = MOVEMENTDIRECTION_NONE;
	facingDirection = FACINGDIRECTION_RIGHT;
	aimAngle = 0.0f;

	power = 0.0f;
	charging = false;
	currentWeapon = NULL;
	weaponStore = NULL;

	hps = 100;
	death = SLUG_DEATH_TIMER;

}

bool Slug::Update(float elapsedTime, Terrain* terrain, const Vector2& gravity, const Vector2& wind)
{

	//
	// AI
	//

	bool moved = false;

	//
	// Find out if we are alive
	//

	if (hps > 0)
	{

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
		// Movement
		//

		if ((movementDirection & MOVEMENTDIRECTION_RIGHT) || (movementDirection & MOVEMENTDIRECTION_LEFT))
		{

			int direction;
			if (movementDirection & MOVEMENTDIRECTION_RIGHT)
				direction = 1;
			else
				direction = -1;

			int pix = (int)position.x + direction;
			int piy = -((int)position.y + radius);

			if (terrain->PointCollision(pix, piy))
			{

				int up = 0;
				while (terrain->PointCollision(pix, piy))
				{

					piy ++;
					up ++;

					if (up > SLUG_MAX_UP_STEP)
						break;

				}

				if (up <= SLUG_MAX_UP_STEP)
				{

					position.x += direction * SLUG_MOVEMENT_SPEED * elapsedTime;
					position.y -= (float)up;
					moved = true;

				}

			}
			else
			{

				int down = 0;
				piy -= 1;

				while (!terrain->PointCollision(pix, piy))
				{

					piy --;

				}

				position.x += direction * SLUG_MOVEMENT_SPEED * elapsedTime;
				moved = true;

				if (down > 0)
					atRest = false;

			}

		}

		//
		// Update aim
		//

		if (movementDirection & MOVEMENTDIRECTION_UP)
		{

			aimAngle -= elapsedTime * SLUG_AIM_SPEED;

			if (aimAngle < -PI_OVER_2)
				aimAngle = -PI_OVER_2;

		}

		if (movementDirection & MOVEMENTDIRECTION_DOWN)
		{

			aimAngle += elapsedTime * SLUG_AIM_SPEED;

			if (aimAngle > PI_OVER_2)
				aimAngle = PI_OVER_2;

		}

		if (moved)
		{

			// Update out sprite since we moved
			Moved();
			atRest = false;

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
	return Object::Update(elapsedTime, terrain, gravity, wind);

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
	// Cancel the resting state and give the slug a velocity in the correction direction
	//

	atRest = false;

	if (facingDirection == FACINGDIRECTION_LEFT)
		velocity.x = -100.0f;
	else
		velocity.x = 100.0f;

	velocity.y = -200.0f;

}

void Slug::Fire()
{

	if (charging)
	{

		//
		// Fire the current weapon
		//

		bool fired;

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

float Slug::GetPower() const
{

	return power;

}

void Slug::Die()
{

	// Overridden for handling by death timer

}

void Slug::Explode()
{

	World::Get()->SimulateExplosion((int)position.x, -(int)position.y, SLUG_EXPLOSION_STRENGTH);

}

void Slug::SpawnGravestone() const
{

	// TODO: Get image resource based on team gravestone style
	Gravestone* gravestone = new Gravestone((ImageResource*)ResourceManager::Get()->GetResource("image_gravestone"));

	if (gravestone)
	{

		// Set to the current position
		gravestone->SetPosition(position);

		// Add the gravestone to the world
		World::Get()->AddCreatedObject(gravestone);

	}

}

void Slug::ArmSelf()
{

	if ((!currentWeapon) && (weaponStore))
		currentWeapon = weaponStore->Get();

}

void Slug::SetWeapons(WeaponStore* store)
{

	weaponStore = store;

	if (currentWeapon)
	{

		currentWeapon = NULL;
		ArmSelf();

	}

}

Team* Slug::GetTeam() const
{

	return team;

}