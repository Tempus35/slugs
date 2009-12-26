#include "particlesystem.h"

ParticleSystem::ParticleSystem(ImageResource* image, float _life)
{

	alive = true;
		
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Update(float elapsedTime)
{

	for (unsigned int i = 0; i < particles.size(); ++ i)
		particles[i].Update(elapsedTime);

}

void ParticleSystem::Render(sf::RenderWindow& renderer)
{

	for (unsigned int i = 0; i < particles.size(); ++ i)
	{

		if (particles[i].Life() > 0.0f)
			particles[i].Render(renderer);

	}

}