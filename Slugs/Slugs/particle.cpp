#include "particle.h"

Particle::Particle()
{

	velocity.x = velocity.y = 0.0f;

}

Particle::~Particle()
{

}

void Particle::Update(float elapsedTime)
{

	if (life > 0.0f)
	{

		sprite.Move(velocity.x * elapsedTime, velocity.y * elapsedTime);

		life -= elapsedTime;

	}

}

void Particle::Render(sf::RenderWindow& renderer)
{

	if (life > 0.0f)
	{

		renderer.Draw(sprite);

	}

}

void Particle::SetImage(ImageResource* imageResource)
{

	sprite.SetImage(imageResource->Image());

}

void Particle::SetPosition(int x, int y)
{

	sprite.SetPosition((float)x, (float)y);

}

void Particle::SetScale(float newScale)
{

	sprite.SetScale(newScale, newScale);

}

void Particle::SetVelocity(float x, float y)
{

	velocity.x = x;
	velocity.y = y;

}

float Particle::Life()
{

	return life;

}

void Particle::SetLife(float newLife)
{

	life = newLife;

}