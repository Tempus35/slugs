#pragma once

#include <list>

#include <SFML/graphics.hpp>

#include "renderer.h"
#include "effect.h"
#include "particlesystem.h"

/*
	class FXManager
*/

class FXManager
{

private:

	std::list<Effect*> effects;

public:

	FXManager();
	~FXManager();

	bool IsRegistered(Effect* effect);
	Effect* RegisterEffect(Effect* effect);
	bool UnregisterEffect(Effect* effect);
	void ClearEffects();

	// Updates all current fx
	void Update(float elapsedTime);

	// Renders all current fx
	void Render();

};