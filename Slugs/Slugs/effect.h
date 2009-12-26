#pragma once

#include "vec2.h"

/*
	class Effect
	Base class fo all effects handled by the FXManager
*/

class Effect
{

protected:

	Vec2f			position;								// World space position of the effect
	float			life;									// Life remaining in seconds

public:

	// Constructor
	Effect(float _life);

	// Updates the effect
	virtual void Update(float elaspedTime) = 0;
	
	// Renders the effect
	virtual void Render(sf::RenderWindow& renderer) = 0;

	// Gets the position of the effect
	virtual const Vec2f& GetPosition() const;

	// Sets the position of the effect
	virtual void SetPosition(const Vec2f& newPosition);

	// Gets the remaining life of the effect
	virtual float GetLife() const;

	// Returns true if the effect is alive
	virtual bool IsAlive() const;

};