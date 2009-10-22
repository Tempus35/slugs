#pragma once

#include "fxmanager.h"
#include "particle.h"
#include "utility.h"

class ParticleSystem : public Effect
{

protected:

	Particle* particles;
	int maxParticles;

public:

	ParticleSystem();
	~ParticleSystem();

	virtual void Update(float elapsedTime);
	virtual void Render(sf::RenderWindow& renderer);

};