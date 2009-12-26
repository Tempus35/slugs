#include "aicontroller.h"
#include "slug.h"

/*
	class AIAction
*/

AIAction::AIAction(AIActionType _type)
{

	type = _type;

}

/*
	class AIController
*/

AIController::AIController(AISkillLevel skillLevel)
{

	skill = skillLevel;

}

void AIController::Update(Slug* slug, float elapsedTime)
{

	ASSERT(slug != NULL);

	if (actionQueue.size() == 0)
		DecideWhatToDo(slug);

}

AISkillLevel AIController::GetSkillLevel() const
{

	return skill;

}

void AIController::DecideWhatToDo(Slug* slug)
{

}