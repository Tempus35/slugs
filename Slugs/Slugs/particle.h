#pragma once

#include <SFML/graphics.hpp>

#include "imageresource.h"
#include "vec2.h"

struct Particle
{

public:

	Vec2f position;
	Vec2f velocity;

	float life;

	sf::Sprite sprite;

public:

	// Constructor
	Particle();

	// Updates the sprite to match the current particle params
	void UpdateSprite();

};