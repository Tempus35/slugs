#include "particlesystem.h"

ParticleSystem::ParticleSystem()
{

	maxParticles = 0;
	particles = NULL;
		
}

ParticleSystem::~ParticleSystem()
{

	SafeDeleteArray(particles);

}

void ParticleSystem::Update(float elapsedTime)
{


	for (int i = 0; i < maxParticles; ++ i)
	{

		particles[i].Update(elapsedTime);

	}

}

void ParticleSystem::Render(sf::RenderWindow& renderer)
{

	for (int i = 0; i < maxParticles; ++ i)
	{

		if (particles[i].Life() > 0.0f)
			particles[i].Render(renderer);

	}

}