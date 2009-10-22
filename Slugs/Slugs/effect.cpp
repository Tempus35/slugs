#include "effect.h"

/*
	class Effect
*/

Effect::Effect()
{

}

Effect::~Effect()
{

}

bool Effect::Continuous()
{

	return continuous;

}

float Effect::Life()
{

	return life;

}

void Effect::SetContinuous(bool isContinuous)
{

	continuous = isContinuous;

}

void Effect::SetLife(float newLife)
{

	life = newLife;

}