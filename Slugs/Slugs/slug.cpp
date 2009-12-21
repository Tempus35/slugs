#include "slug.h"
#include "team.h"

#include "resourcemanager.h"

Slug::Slug() : Object()
{

	type = OBJECTTYPE_SLUG;

	movementDirection = MOVEMENTDIRECTION_NONE;
	facingDirection = FACINGDIRECTION_RIGHT;
	viewAngle = 0.0f;

	power = 0.0f;
	charging = false;
	currentWeapon = NULL;
	weaponStore = NULL;

	hps = 100;
	death = SLUG_DEATH_TIMER;

}

bool Slug::Update(float elapsedTime, Terrain* terrain, const Vector2& gravity, const Vector2& wind)
{

	// AI

	bool moved = false;

	if (hps > 0)
	{

		/*
			 The slug is alive
		*/

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

			viewAngle -= elapsedTime * SLUG_AIM_SPEED;

			if (viewAngle < -PI_OVER_2)
				viewAngle = -PI_OVER_2;

		}

		if (movementDirection & MOVEMENTDIRECTION_DOWN)
		{

			viewAngle += elapsedTime * SLUG_AIM_SPEED;

			if (viewAngle > PI_OVER_2)
				viewAngle = PI_OVER_2;

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

			// Go boom when our timer runs out
			Explode();
			alive = false;

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

		if (currentWeapon)
			currentWeapon->Fire();

		Projectile* bomb = new Projectile();
		bomb->SetPosition(Vector2(position.x, position.y));
		bomb->SetOwner(this);
		bomb->SetStrength(75);
		bomb->SetTimer(-1);
		bomb->SetRadius(5);
		bomb->SetImage(((ImageResource*)ResourceManager::Get()->GetResource("image_gravestone")));

		float shotVelocity = power * SHOT_POWER_MULTIPLIER;
		float cosAnglePower = cosf(viewAngle) * shotVelocity;
		float sinAnglePower = sinf(viewAngle) * shotVelocity;

		if (facingDirection == FACINGDIRECTION_RIGHT)
			bomb->SetVelocity(cosAnglePower, sinAnglePower);
		else
			bomb->SetVelocity(-cosAnglePower, sinAnglePower);

		// Add the prjectile to the world
		World::Get()->AddCreatedObject(bomb);

		// We are no longer charging the weapn
		charging = false;

	}

}

float Slug::ViewAngle()
{

	return viewAngle;

}

FaceDirection Slug::FacingDirection()
{

	return facingDirection;

}

void Slug::Die()
{

	// Overridden for handling by death timer

}

void Slug::Explode()
{

	World::Get()->SimulateExplosion((int)position.x, -(int)position.y, SLUG_EXPLOSION_STRENGTH);

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