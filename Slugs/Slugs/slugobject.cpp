#include "slugobject.h"

#include "resourcemanager.h"

SlugObject::SlugObject() : GameObject()
{

	type = GAMEOBJECTTYPE_SLUG;

	movementDirection = MOVEMENTDIRECTION_NONE;
	facingDirection = FACINGDIRECTION_RIGHT;

	viewAngle = 0.0f;

	power = 0.0f;
	charging = false;

	hps = 100;
	death = SLUG_DEATH_TIMER;

}

bool SlugObject::Update(float elapsedTime, Terrain* terrain, Vector2 gravity, Vector2 wind)
{

	// AI

	bool moved = false;

	if (hps > 0)
	{

	// Update shot power if slug is charging a weapon
	if (charging)
	{

		power += elapsedTime * SHOT_POWER_CHARGE_RATE;

		if (power >= 1.0f)
			Fire();
			
	}
	
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

		Moved();
		atRest = false;

	}

	}
	else
	{

		death -= elapsedTime;

		if (death <= 0.0f)
		{

			Explode();
			alive = false;

		}

	}

	// Physics
	return GameObject::Update(elapsedTime, terrain, gravity, wind);

}

void SlugObject::StartMovingLeft()
{

	movementDirection |= MOVEMENTDIRECTION_LEFT;
	facingDirection = FACINGDIRECTION_LEFT;

}	

void SlugObject::StartMovingRight()
{

	movementDirection |= MOVEMENTDIRECTION_RIGHT;
	facingDirection = FACINGDIRECTION_RIGHT;

}

void SlugObject::StopMoving()
{

	movementDirection &= ~(MOVEMENTDIRECTION_LEFT|MOVEMENTDIRECTION_RIGHT);
	
}

void SlugObject::StartAimingUp()
{

	movementDirection |= MOVEMENTDIRECTION_UP;

}

void SlugObject::StartAimingDown()
{

	movementDirection |= MOVEMENTDIRECTION_DOWN;

}

void SlugObject::StopAiming()
{

	movementDirection &= ~(MOVEMENTDIRECTION_UP|MOVEMENTDIRECTION_DOWN);

}

void SlugObject::StartCharging()
{

	if (!charging)
	{

		power = 0.0f;
		charging = true;

	}

}

void SlugObject::Jump()
{

	atRest = false;

	if (facingDirection == FACINGDIRECTION_LEFT)
		velocity.x = -100.0f;
	else
		velocity.x = 100.0f;

	velocity.y = -200.0f;

}

void SlugObject::Fire()
{

	if (charging)
	{

		WeaponObject* bomb = new WeaponObject();
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

		World::Get()->AddCreatedObject(bomb);

		charging = false;

	}

}

float SlugObject::ViewAngle()
{

	return viewAngle;

}

FaceDirection SlugObject::FacingDirection()
{

	return facingDirection;

}

void SlugObject::Die()
{

	// Overridden for handling by death timer

}

void SlugObject::Explode()
{

	World::Get()->SimulateExplosion((int)position.x, -(int)position.y, SLUG_EXPLOSION_STRENGTH);

}