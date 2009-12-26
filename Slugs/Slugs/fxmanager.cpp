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

	std::list<Effect*>::iterator i = effects.begin();

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

	std::list<Effect*>::iterator i = effects.begin();

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

	std::list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		delete (*i);

		i ++;

	}

	effects.clear();

}

void FXManager::Update(float elapsedTime)
{

	std::list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		Effect* effect = (Effect*)(*i);

		if (effect->IsAlive())
		{

			effect->Update(elapsedTime);
			i ++;

		}
		else
		{

			SafeDelete(effect);
			i = effects.erase(i);

		}

	}

}

void FXManager::Render()
{

	std::list<Effect*>::iterator i = effects.begin();

	while (i != effects.end())
	{

		(*i)->Render(Renderer::Get()->GetWindow());

		i ++;

	}

}