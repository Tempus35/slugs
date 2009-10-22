#pragma once

#include <list>
using namespace std;

#include <SFML/graphics.hpp>

#include "singleton.h"
#include "effect.h"

/*
	class FXManager
*/

class FXManager : public Singleton<FXManager>
{

friend class Singleton<FXManager>;

private:

	list<Effect*> effects;

	FXManager();
	~FXManager();

public:

	bool IsRegistered(Effect* effect);
	Effect* RegisterEffect(Effect* effect);
	bool UnregisterEffect(Effect* effect);
	void ClearEffects();

	void Update(float elapsedTime);
	void Render();

};