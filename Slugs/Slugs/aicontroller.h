#pragma once

#include <queue>

#include "debug.h"
#include "aiaction.h"
#include "weapon.h"

/*
	Enumeration of AI skill levels
*/

enum AISkillLevel
{

	AISkillLevel_Novice,
	AISkillLevel_Average,
	AISkillLevel_Expert,

};

/* 
	Forward declarations
*/

class Slug;

/*
	class AIController
	Slug AI controller. This class is the brain behind a computer controlled slug.
*/

class AIController
{

protected:

	AISkillLevel				skill;			// Skill level of this controller
	std::queue<AIAction*>		actionQueue;	// Queue of actions to process

	std::vector<Object*>		validTargets;
	std::vector<Object*>		invalidTargets;
	std::vector<Object*>		badTargets;
	std::vector<Object*>		farTargets;

protected:

	// Decides what the slug will try to accomplish this turn and creates an action queue
	void DecideWhatToDo(Slug* slug);

public:

	// Constructor
	AIController(AISkillLevel skillLevel = AISkillLevel_Expert);

	// Destructor
	~AIController();

	// Main update function for the controller
	virtual void Update(Slug* slug, float elapsedTime);

	// Returns the skill level of the controller
	AISkillLevel GetSkillLevel() const;

	// Renders debugging info
	void DebugRender();

};