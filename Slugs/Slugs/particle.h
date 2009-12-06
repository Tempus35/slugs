#pragma once

#include <SFML/graphics.hpp>

#include "imageresource.h"
#include "vector2.h"

class Particle
{

private:

	sf::Sprite sprite;
	Vector2 velocity;

	float life;

public:

	Particle(ImageResource* imageResource);
	Particle();
	~Particle();

	void Update(float elapsedTime);
	void Render(sf::RenderWindow& renderer);

	void SetImage(ImageResource* imageResource);

	void SetPosition(int x, int y);
	void SetScale(float newScale);
	void SetVelocity(float x, float y);

	float Life();
	void SetLife(float newLife);

};