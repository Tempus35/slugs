#include "fxmanager.h"

/*
	class FXManager
*/

FXManager::FXManager()
{

}

FXManager::~FXManager()
{

}

bool FXManager::IsRegistered(Effect* effect)
{

	list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		if ((*i) == effect)
			return true;

		i ++;

	}

	return false;

}

Effect* FXManager::RegisterEffect(Effect* effect)
{

	if (!IsRegistered(effect))
		effects.push_back(effect);
		
	return effect;

}
bool FXManager::UnregisterEffect(Effect* effect)
{

	list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		if ((*i) == effect)
		{

			delete (*i);

			effects.erase(i);
			return true;

		}

		i ++;

	}

	return false;

}

void FXManager::ClearEffects()
{

	list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		delete (*i);

		i ++;

	}

	effects.clear();

}

void FXManager::Update(float elapsedTime)
{

	list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		(*i)->Update(elapsedTime);

		i ++;

	}

}

void FXManager::Render()
{

	list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		(*i)->Render();

		i ++;

	}

}