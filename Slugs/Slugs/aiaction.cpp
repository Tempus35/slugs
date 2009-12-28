#include "aiaction.h"

/*
	class AIAction
*/

AIAction::AIAction(AIActionType _type)
{

	type = _type;

}

/*
	class AICommandAction
*/

AICommandAction::AICommandAction(AIActionCommandType _command) : AIAction(AIActionType_Command)
{

	command = _command;

}

/*
	class AIAttackAction
*/

AIAttackAction::AIAttackAction(WeaponType _weapon, Object* _target, const Vec2f& _direction, float _speed) : AIAction(AIActionType_Attack)
{

	weapon = _weapon;
	target = _target;
	direction = _direction;
	speed = speed;

}

/*
	class AIPauseAction
*/

AIPauseAction::AIPauseAction(float duration) : AIAction(AIActionType_Pause)
{

	timer = duration;

}
