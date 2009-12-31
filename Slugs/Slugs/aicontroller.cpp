#include "aicontroller.h"
#include "slug.h"
#include "game.h"

/*
	class AIController
*/

AIController::AIController(AISkillLevel skillLevel)
{

	skill = skillLevel;

}

AIController::~AIController()
{

	// Free any actions left over
	while (actionQueue.size() > 0)
	{

		SafeDelete(actionQueue.front());
		actionQueue.pop();

	}

}

void AIController::Update(Slug* slug, float elapsedTime)
{

	ASSERT(slug != NULL);

	if (actionQueue.size() == 0)
		DecideWhatToDo(slug);

	// Get the action at the head of the queue
	AIAction* currentAction = actionQueue.front();
	bool actionFinished = false;
	bool endTurn = false;

	//
	// Process the action
	//

	if (currentAction->type == AIActionType_Command)
	{

		AICommandAction* commandAction = (AICommandAction*)currentAction;
		
		switch (commandAction->command)
		{

		case AIActionCommandType_EndTurn:
			endTurn = true;
			break;

		}

	}
	else if (currentAction->type == AIActionType_Attack)
	{

		AIAttackAction* attackAction = (AIAttackAction*)currentAction;

		Weapon* weapon = slug->ArmSelf(attackAction->weapon);

		Vec2f aimDirection = slug->GetAimDirection();

		float angle = Acos(DotProduct(attackAction->direction, aimDirection));

		ASSERT(IsNumber(angle));
		ASSERT(IsFinite(angle));

		if (slug->StartAimingTowards(attackAction->direction))
		{

			//
			// Fire
			//

			slug->StopAiming();
			slug->StartCharging();

			if (weapon->GetLaunchSpeed(slug->GetPower()) >= attackAction->speed)
			{

				slug->SetPower(attackAction->speed / 1500.0f);
				slug->Fire();
				actionFinished = true;

			}

		}

	}
	else if (currentAction->type == AIActionType_Pause)
	{

		AIPauseAction* pauseAction = (AIPauseAction*)currentAction;
		pauseAction->timer -= elapsedTime;

		if (pauseAction->timer <= 0.0f)
			actionFinished = true;

	}

	// If we finished the action at the head of the queue, pop it off
	if (actionFinished)
	{

		SafeDelete(actionQueue.front());
		actionQueue.pop();

	}

	//
	// End the turn if we need to
	//

	if ((endTurn) || (actionQueue.size() == 0))
	{

		// Clear any remaining actions
		while (actionQueue.size() > 0)
		{
			SafeDelete(actionQueue.front());
			actionQueue.pop();
		}

		// End the turn
		slug->GetTeam()->GetPlayer()->EndTurn();

	}


}

AISkillLevel AIController::GetSkillLevel() const
{

	return skill;

}

void AIController::DecideWhatToDo(Slug* slug)
{

	validTargets.clear();
	invalidTargets.clear();
	badTargets.clear();
	farTargets.clear();

	std::vector<Object*> objects;
	Game::Get()->GetWorld()->GetObjectsNear(objects, slug, Math::INFINITY);

	//
	// Search for an enemy
	//

	Object* closestEnemy = NULL;
	float closestEnemyDistance = Math::INFINITY;
	Vec2f closestEnemyDirection;
	Object* closestEnemyIndirect = NULL;
	float closestEnemyDistanceIndirect = Math::INFINITY;
	float optimalSpeed, speed;
	Vec2f optimalDirection, direction;

	for (unsigned int i = 0; i < objects.size(); ++ i)
	{

		Object* object = objects[i];

		if (object->GetType() == ObjectType_Slug)
		{

			Slug* p = (Slug*)object;

			// Make sure we aren't on the same team or dead and still in the world
			if ((p->GetTeam()->GetPlayer() != slug->GetTeam()->GetPlayer()) && (p->GetHitPoints() > 0))
			{

				Vec2f delta = object->GetPosition() - slug->GetPosition();
				float d = delta.LengthSquared();

				// Check range
				if (d < closestEnemyDistance)
				{

					// Check for line of sight
					if (Game::Get()->GetWorld()->ObjectCanSeeDirect(slug, object))
					{

						closestEnemyDistance = d;
						closestEnemy = object;
						closestEnemyDirection = delta.Normalize();

					}

				}

				if (d < closestEnemyDistanceIndirect)
				{

					// Ensure we aren't so close that we would take damage if we shot this object
					if (d > 100 * 100)
					{

						// Check for parabolic line of sight
						if (Game::Get()->GetWorld()->ObjectCanShootIndirect(slug, object, 1500.0f, direction, speed))
						{

							optimalDirection = direction;

							optimalSpeed = speed;
							closestEnemyDistanceIndirect = d;
							closestEnemyIndirect = object;

							validTargets.push_back(object);

						}
						else
							invalidTargets.push_back(object);

					}
					else
						badTargets.push_back(object);

				}
				else
					farTargets.push_back(object);

			}

		}

	}

	// Insert a pause to give the camera a chance to move to the slug
	actionQueue.push(new AIPauseAction(2.0f));

	//
	// Attack an enemy
	//

	//if (closestEnemy == NULL)
	//{

		if (closestEnemyIndirect == NULL)
		{

			slug->SetGoal(NULL);
			actionQueue.push(new AICommandAction(AIActionCommandType_EndTurn));

		}
		else
		{

			ASSERT(optimalDirection.Length() > Math::EPSILON);

			WeaponType weaponType = WeaponType_Bazooka;

			// Throw in a few grenades if appropriate
			if ((closestEnemyIndirect->GetPosition().y < slug->GetPosition().y) && (closestEnemyDistanceIndirect > 500 * 500) && (Random::RandomFloat() < 0.9f))
				weaponType = WeaponType_Bazooka;

			slug->SetGoal(closestEnemyIndirect);
			actionQueue.push(new AIAttackAction(weaponType, closestEnemyIndirect, optimalDirection, optimalSpeed));

		}
	
	/*
	}
	else
	{

		if (closestEnemyDistance < 100.0f)
			weaponType = WeaponType_Machinegun;
		else
			weaponType = WeaponType_Bazooka;

		slug->SetGoal(closestEnemy);
		actionQueue.push(new AIAttackAction(weaponType, closestEnemy, closestEnemyDirection, 2000.0f));

	}
	*/

}

void AIController::DebugRender()
{

	for (unsigned int i = 0; i < validTargets.size(); ++ i)
		Renderer::Get()->DrawDebugCircle(validTargets[i]->GetPosition(), 10.0f, Color::green);

	for (unsigned int i = 0; i < invalidTargets.size(); ++ i)
		Renderer::Get()->DrawDebugCircle(invalidTargets[i]->GetPosition(), 10.0f, Color::gray);

	for (unsigned int i = 0; i < badTargets.size(); ++ i)
		Renderer::Get()->DrawDebugCircle(badTargets[i]->GetPosition(), 10.0f, Color::red);

	for (unsigned int i = 0; i < farTargets.size(); ++ i)
		Renderer::Get()->DrawDebugCircle(farTargets[i]->GetPosition(), 10.0f, Color::blue);

}