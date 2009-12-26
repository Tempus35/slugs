#pragma once

#include <queue>

#include "debug.h"

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
	Enumeration of AI action types
*/

enum AIActionType
{

	AIActionType_None,
	AIActionType_Pause,

};

/* 
	Forward declarations
*/

class Slug;

/*
	struct AIAction
	Base class for AIActions. These are events that are stored in a queue and processed by the AI update.
*/

class AIAction
{

public:

	AIActionType type;							// Type of this action

public:

	AIAction(AIActionType _type);

};

/*
	class AIController
	Slug AI controller. This class is the brain behind a computer controlled slug.
*/

class AIController
{

protected:

	AISkillLevel				skill;			// Skill level of this controller
	std::queue<AIAction*>		actionQueue;	// Queue of actions to process

protected:

	// Decides what the slug will try to accomplish this turn and creates an action queue
	void DecideWhatToDo(Slug* slug);

public:

	// Constructor
	AIController(AISkillLevel skillLevel);

	// Main update function for the controller
	virtual void Update(Slug* slug, float elapsedTime);

	// Returns the skill level of the controller
	AISkillLevel GetSkillLevel() const;

};