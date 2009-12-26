#include "effect.h"

/*
	class Effect
*/

Effect::Effect(float _life)
{

	life = _life;

}

const Vec2f& Effect::GetPosition() const
{

	return position;

}

void Effect::SetPosition(const Vec2f& newPosition)
{

	position = newPosition;

}

float Effect::GetLife() const
{

	return life;

}

bool Effect::IsAlive() const
{

	return (life > 0.0f);

}