#pragma once

#include "vec2.h"
#include "object.h"
#include "weapon.h"

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
	class AIPauseAction
	A pause
*/

class AIPauseAction : public AIAction
{

public:

	float timer;

public:

	AIPauseAction(float duration);

};
