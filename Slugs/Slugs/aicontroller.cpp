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

		Renderer::Get()->DrawDebugCircle(attackAction->target->GetPosition(), 10.0f, Color(0, 255, 0));

		slug->ArmSelf(attackAction->weapon);

		Vec2f aimDirection = slug->GetAimDirection();

		float angle = Acos(DotProduct(attackAction->direction, aimDirection));

		if (slug->StartAimingTowards(attackAction->direction))
		{

			//
			// Fire
			//

			slug->StopAiming();

			slug->StartCharging();
			actionFinished = true;

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
	float optimalSpeed;
	Vec2f optimalDirection, direction;

	for (unsigned int i = 0; i < objects.size(); ++ i)
	{

		Object* object = objects[i];

		if (object->GetType() == ObjectType_Slug)
		{

			Slug* p = (Slug*)object;

			if (p->GetTeam()->GetPlayer() != slug->GetTeam()->GetPlayer())
			{

				Vec2f delta = object->GetPosition() - slug->GetPosition();
				float d = delta.LengthSquared();

				// Check range
				if (d < closestEnemyDistance)
				{

					// Check for line of sight
					if (Game::Get()->GetWorld()->ObjectCanSee(slug, object))
					{

						closestEnemyDistance = d;
						closestEnemy = object;
						closestEnemyDirection = delta.Normalize();

					}

				}

				if (d < closestEnemyDistanceIndirect)
				{

					// Check for parabolic line of sight
					if (Game::Get()->GetWorld()->ObjectCanSeeParabolic(slug, object, direction, optimalSpeed))
					{

						optimalDirection = direction;
						closestEnemyDistanceIndirect = d;
						closestEnemyIndirect = object;

					}

				}

			}

		}

	}

	// Insert a pause to give the camera a chance to move to the slug
	actionQueue.push(new AIPauseAction(2.0f));

	//
	// Attack an enemy
	//

	if (closestEnemy == NULL)
	{

		if (closestEnemyIndirect == NULL)
		{

			slug->SetGoal(NULL);
			actionQueue.push(new AICommandAction(AIActionCommandType_EndTurn));

		}
		else
		{

			slug->SetGoal(closestEnemyIndirect);
			actionQueue.push(new AIAttackAction(WeaponType_Bazooka, closestEnemyIndirect, optimalDirection, optimalSpeed));

		}
	
	}
	else
	{

		WeaponType weaponType;

		if (closestEnemyDistance < 100.0f)
			weaponType = WeaponType_Machinegun;
		else
			weaponType = WeaponType_Bazooka;

		slug->SetGoal(closestEnemy);
		actionQueue.push(new AIAttackAction(weaponType, closestEnemy, closestEnemyDirection, 2000.0f));

	}

}