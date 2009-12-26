#include "particle.h"

Particle::Particle()
{

}

void Particle::UpdateSprite()
{

	sprite.SetPosition(position.x, -position.y);

}