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

	goal = NULL;
	stunTimer = 0.0f;

	// Add an attachpoint and attach a hat
	AddAttachpoint(Vec2f(3.0f, 7.0f));
	AddAttachment(0, new Attachment(ResourceManager::Get()->GetImage("image_hat0")));

}

Slug::~Slug()
{

	if (controller)
		SafeDelete(controller);

	if (ownsWeaponStore)
		SafeDelete(weaponStore);

}

bool Slug::Update(float elapsedTime, const Vec2f& gravity, const Vec2f& wind)
{

	const float SLUG_MAX_UP_STEP = 5.0f;			// Maximum number of pixels a slug can move up per pixel moved left/right
	const float SLUG_MAX_DOWN_STEP = 10.0f;			// Maximum number of pixels a slug can move down without falling
	const float SLUG_MOVEMENT_SPEED = 50.0f;		// Slug movement speed in pixels/second
	const float SLUG_AIM_SPEED = Radians(45.0f);	// Rate at which the slug can aim in radians/second
	const float SHOT_POWER_CHARGE_RATE = 0.5f;		// Rate of weapon charge in units/second	

	//
	// Find out if we are alive
	//

	bool moved = false;

	if (hps > 0)
	{

		// If we are stunned we can't perform any actions
		if (stunTimer > 0.0f)
		{

			stunTimer -= elapsedTime;

		}
		else
		{

			// Update our ai controller if we have one. This must be the active slug on the active player.
			if ((controller) && (team->GetPlayer() == Game::Get()->GetCurrentPlayer()) && (team->GetPlayer()->GetCurrentSlug() == this) && (!team->GetPlayer()->IsTurnEnding()))
				controller->Update(this, elapsedTime);

			//
			// Update shot power if slug is charging a weapon
			//

			if (charging)
			{

				power += elapsedTime * SHOT_POWER_CHARGE_RATE;

				// Fire when we reach max power
				if (power >= 1.0f)
				{
					power = 1.0f;
					Fire();
				}
					
			}
			
			//
			// Movement - only works when in the resting state
			//

			if (atRest)
			{

				float direction = 0.0f;

				if (movementDirection & MOVEMENTDIRECTION_RIGHT)
					direction = 1.0f;
				else if (movementDirection & MOVEMENTDIRECTION_LEFT)
					direction = -1.0f;

				if (direction != 0.0f)
				{

					float height = Game::Get()->GetWorld()->GetTerrain()->GetHeightAt(Vec2f(bounds.center.x + direction, bounds.center.y));
			
					float dy = height - (bounds.center.y - bounds.extents.y);

					if ((dy >= 0.0f) && (dy <= SLUG_MAX_UP_STEP))
					{
					
						moved = true;
						bounds.center.x += direction * elapsedTime * SLUG_MOVEMENT_SPEED;
						bounds.center.y = height + RoundUpToInt(bounds.extents.y) + 2.0f;
					
					}
					else if (dy < 0.0f)
					{

						if (Abs(dy) <= SLUG_MAX_DOWN_STEP)
						{

							moved = true;
							bounds.center.x += direction * elapsedTime * SLUG_MOVEMENT_SPEED;
							bounds.center.y = height + bounds.extents.y + 2.0f;

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

bool Slug::OnCollideWithTerrain()
{

	//
	// Apply falling damage
	//

	if (Game::Get()->GetGameBool(GameBool_FallDamage))
	{

		const float SAFE_FALL_HEIGHT = 125.0f;

		float fallHeight = initialHeight - bounds.center.y;

		if (fallHeight > SAFE_FALL_HEIGHT)
		{

			AdjustHitpoints(-RoundDownToInt(fallHeight / 10.0f));

		}

	}

	// Set current height as the new initial height
	initialHeight = bounds.center.y;

	return Object::OnCollideWithTerrain();

}

void Slug::OnHitpointsChanged(int oldValue)
{

	if ((Game::Get()->GetGameBool(GameBool_DamageEndsTurn)) && (oldValue > hps))
	{

		// Stun the slug so it can't use its end turn time to move
		StunSelf(Game::Get()->GetGameFloat(GameFloat_TurnEndTime));

		team->GetPlayer()->EndTurn();

	}

}

void Slug::StartMovingLeft()
{

	GetPlayer()->Acted();

	if (stunTimer <= 0.0f)
	{

		movementDirection |= MOVEMENTDIRECTION_LEFT;
		FaceLeft();

	}

}	

void Slug::StartMovingRight()
{

	GetPlayer()->Acted();

	if (stunTimer <= 0.0f)
	{

		movementDirection |= MOVEMENTDIRECTION_RIGHT;
		FaceRight();

	}

}

void Slug::StopMoving()
{

	movementDirection &= ~(MOVEMENTDIRECTION_LEFT|MOVEMENTDIRECTION_RIGHT);
	
}

void Slug::StartAimingUp()
{

	GetPlayer()->Acted();

	if (stunTimer <= 0.0f)
	{

		movementDirection |= MOVEMENTDIRECTION_UP;
		movementDirection &= ~MOVEMENTDIRECTION_DOWN;

	}

}

void Slug::StartAimingDown()
{

	GetPlayer()->Acted();

	if (stunTimer <= 0.0f)
	{

		movementDirection |= MOVEMENTDIRECTION_DOWN;
		movementDirection &= ~MOVEMENTDIRECTION_UP;

	}

}

bool Slug::StartAimingTowards(const Vec2f& direction)
{

	GetPlayer()->Acted();

	if (stunTimer > 0.0f)
		return false;

	Vec2f aimDirection = GetAimDirection();

	float dp = DotProduct(direction, aimDirection);

	// Adjust facing if needed
	if (SignCompare(direction.x, aimDirection.x) == false)
	{

		if (facingDirection == FACINGDIRECTION_RIGHT)
			FaceLeft();
		else
			FaceRight();

	}
	else
	{

		float angle = Acos(dp);

		if (angle > Math::PI_OVER_180 * 2.0f)
		{

			if (aimDirection.y > direction.y)
				StartAimingDown();
			else
				StartAimingUp();

		}
		else
		{

			//
			// We are close enough, just adjust the aim angle directly
			//

			if (aimDirection.y > direction.y)
				aimAngle -= angle;
			else
				aimAngle += angle;

			return true;

		}

	}

	return false;

}

void Slug::StopAiming()
{

	movementDirection &= ~(MOVEMENTDIRECTION_UP|MOVEMENTDIRECTION_DOWN);

}

void Slug::StopEverything()
{

	StopMoving();
	StopAiming();

	goal = NULL;
	power = 0.0f;

}

void Slug::StartCharging()
{

	GetPlayer()->Acted();

	if (!charging)
	{

		if (currentWeapon != NULL)
		{

			if (currentWeapon->NeedsCharge() == true)
			{

				power = 0.0f;
				charging = true;

			}
			else
				Fire();

		}

	}

}

void Slug::Jump()
{

	GetPlayer()->Acted();

	//
	// We can only jump if we are in the resting state
	// Cancel the resting state and give the slug a velocity in the correct direction
	//

	if (atRest)
	{
	
		SetAtRest(false);

		if (facingDirection == FACINGDIRECTION_LEFT)
			velocity.x = -125.0f;
		else
			velocity.x = 125.0f;

		velocity.y = 300.0f;

	}

}

void Slug::Fire()
{

	GetPlayer()->Acted();

	if (currentWeapon)
	{

		if ((charging) || (!currentWeapon->NeedsCharge()))
		{

			// We are no longer charging the weapon
			charging = false;

			//
			// Fire the current weapon
			//

			bool fired = false;

			Projectile* projectile = NULL;
			fired = currentWeapon->Fire(this, projectile);

			if (projectile)
				Game::Get()->GetCamera()->StartTracking(projectile);

			if (fired)
			{

				// Let our owning player know we fired
				GetTeam()->GetPlayer()->SlugFired();

			}
			else
			{

				// TODO: Didn't fire, do whatever we need to do

			}

		}

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

Vec2f Slug::GetAimDirection() const
{

	if (facingDirection == FACINGDIRECTION_RIGHT)
		return Vec2f(Cos(aimAngle), Sin(aimAngle)).Normalize();
	else
		return Vec2f(-Cos(aimAngle), Sin(aimAngle)).Normalize();

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

void Slug::SetPower(float newPower)
{

	power = newPower;

}

void Slug::Die(bool instant)
{

	if (instant)
	{
		hps = -1;
		alive = false;
	}

}

void Slug::Explode()
{

	const ExplosionData explosionData(30.0f, 35.0f, 35.0f, 30.0f, 30.0f);

	Game::Get()->GetWorld()->SimulateExplosion(bounds.center, explosionData);

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

Weapon* Slug::ArmSelf()
{

	if (!charging)
	{

		if ((!currentWeapon) && (weaponStore))
			currentWeapon = weaponStore->Get();

	}

	return currentWeapon;

}

Weapon* Slug::ArmSelf(WeaponType type)
{

	GetPlayer()->Acted();

	if (!charging)
	{

		if (currentWeapon->GetType() != type)
			currentWeapon = weaponStore->Get(type);

	}

	return currentWeapon;

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

	return GetPosition();

	// TODO

	Vec2f aimDirection = Vec2f(Cos(aimAngle), Sin(aimAngle));

	if (facingDirection != FACINGDIRECTION_RIGHT)
		aimDirection.x = -aimDirection.x;

	return bounds.center + aimDirection * bounds.extents.x;

}

Team* Slug::GetTeam() const
{

	return team;

}

Player* Slug::GetPlayer() const
{

	return team->GetPlayer();

}

void Slug::FaceRight()
{

	GetPlayer()->Acted();

	if (facingDirection != FACINGDIRECTION_RIGHT)
	{

		facingDirection = FACINGDIRECTION_RIGHT;
		SetImage((ImageResource*)ResourceManager::Get()->GetResource("image_slug_right"));

	}

}

void Slug::FaceLeft()
{

	GetPlayer()->Acted();

	if (facingDirection != FACINGDIRECTION_LEFT)
	{

		facingDirection = FACINGDIRECTION_LEFT;
		SetImage((ImageResource*)ResourceManager::Get()->GetResource("image_slug_left"));

	}

}

void Slug::SetGoal(Object* object)
{

	goal = object;

}

void Slug::Render()
{

	// Render object sprite
	Renderer::Get()->Render(sprite);

	// Render attachments
	for (unsigned int i = 0; i < attachPoints.size(); ++ i)
	{

		for (unsigned int j = 0; j < attachPoints[i].attachments.size(); ++ j)
		{

			if (facingDirection == FACINGDIRECTION_RIGHT)
				attachPoints[i].attachments[j]->SetMirrored(false);
			else
				attachPoints[i].attachments[j]->SetMirrored(true);

			attachPoints[i].attachments[j]->Render(GetPosition() + TransformAttachmentOffset(attachPoints[i].offset));

		}

	}

	if (IsActive())
	{

		// Draw crosshair if weapon requires aiming
		if ((currentWeapon->RequiresAiming()) && (hps > 0))
		{

			const float CROSSHAIR_DISTANCE = 60.0f;

			Vec2f crosshairPosition = GetPosition() + GetAimDirection() * CROSSHAIR_DISTANCE;
			Game::Get()->GetWorld()->SetCrosshairPosition(crosshairPosition);		
			Game::Get()->GetWorld()->SetCrosshairVisible(true);

		}
		else
			Game::Get()->GetWorld()->SetCrosshairVisible(false);

		// Draw current target for targetable weapons
		if (currentWeapon->IsTargetable())
		{

			TargetableWeapon* weapon = (TargetableWeapon*)currentWeapon;

			if (weapon->HasTarget())
			{

				Renderer::Get()->DrawDebugCircle(weapon->GetTarget(), 20.0f, Color::green);
				Renderer::Get()->DrawDebugCircle(weapon->GetTarget(), 10.0f, Color::green);

			}

		}

	}

}

void Slug::DebugRender()
{

	if (goal != NULL)
	{

		// Goal arrow for AI controlled slugs
		Renderer::Get()->DrawDebugArrow(GetPosition(), goal->GetPosition(), Color(255, 0, 0));

	}

	// Debugging for the active slug
	if (IsActive())
	{

		// Aiming trajectory
		if (power > 0.0f)
			Renderer::Get()->DrawDebugTrajectory(GetPosition(), GetAimDirection(), currentWeapon->GetLaunchSpeed(power), Game::Get()->GetWorld()->Gravity().Length(), Color(0, 0, 255));

		// Weapon debug
		currentWeapon->DebugRender();

		// AI debugging
		if (controller)
			controller->DebugRender();

	}
	
	// Draw object debugging info (bounds, etc)
	Object::DebugRender();

}

void Slug::StunSelf(float duration)
{

	stunTimer = duration;

}

void Slug::SetTarget(const Vec2f& position)
{

	if (currentWeapon->IsTargetable())
	{

		TargetableWeapon* weapon = (TargetableWeapon*)currentWeapon;
		weapon->SetTargetPoint(position);

	}

}

bool Slug::IsActive() const
{

	if ((team->GetPlayer()->GetCurrentSlug() == this) && (Game::Get()->IsActivePlayer(team->GetPlayer())))
		return true;

	return false;

}

Vec2f Slug::TransformAttachmentOffset(const Vec2f& offset)
{

	if (facingDirection == FACINGDIRECTION_LEFT)
		return Vec2f(-offset.x, offset.y);

	return offset;

}

void Slug::GiveWeapon(WeaponType weaponType, int ammo)
{

	if (weaponStore)
		weaponStore->Add(weaponType, ammo);

}