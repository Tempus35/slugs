#pragma once

#include <list>
using namespace std;

#include <SFML/graphics.hpp>

#include "effect.h"

/*
	class FXManager
*/

class FXManager
{

private:

	list<Effect*> effects;

public:

	FXManager();
	~FXManager();

	bool IsRegistered(Effect* effect);
	Effect* RegisterEffect(Effect* effect);
	bool UnregisterEffect(Effect* effect);
	void ClearEffects();

	void Update(float elapsedTime);
	void Render();

};