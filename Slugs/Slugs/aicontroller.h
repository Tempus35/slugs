#pragma once

#include <queue>

#include "debug.h"
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
	Enumeration of AI action types
*/

enum AIActionType
{

	AIActionType_None,
	AIActionType_Pause,
	AIActionType_Command,
	AIActionType_Attack,

};

enum AIActionCommandType
{

	AIActionCommandType_EndTurn,

};

/* 
	Forward declarations
*/

class Slug;

/*
	class AIAction
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
	class AICommandAction
	Simple command
*/

class AICommandAction : public AIAction
{

public:

	AIActionCommandType command;

public:

	AICommandAction(AIActionCommandType _command);

};

/*
	class AIAttackAction
	Attacks an object with a weapon
*/

class AIAttackAction : public AIAction
{

public:

	WeaponType	weapon;
	Object*		target;
	Vec2f		direction;
	float		speed;

public:

	AIAttackAction(WeaponType _weapon, Object* _target, const Vec2f& _direction, float _speed);

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
	AIController(AISkillLevel skillLevel = AISkillLevel_Expert);

	// Destructor
	~AIController();

	// Main update function for the controller
	virtual void Update(Slug* slug, float elapsedTime);

	// Returns the skill level of the controller
	AISkillLevel GetSkillLevel() const;

};