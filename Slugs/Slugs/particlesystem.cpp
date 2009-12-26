#include "particlesystem.h"

ParticleSystem::ParticleSystem(ImageResource* _imageResource, float _life) : Effect(_life)
{

	ASSERT(_imageResource);

	numParticles = 0;

	imageResource = _imageResource;

	spawnTimer = 0.0f;
	spawnTime = 0.005f;
		
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Update(float elapsedTime)
{

	if ((life > 0.0f) || (numParticles > 0))
	{

		//
		// Update existing particles
		//

		int i = 0;

		while (i < numParticles)
		{

			if (particles[i].life > 0.0f)
			{

				UpdateParticle(&particles[i], elapsedTime);
				i ++;

			}
			else
			{

				// Copy last particle to this one
				particles[i] = particles[numParticles - 1];

				numParticles --;

			}

		}

		if (life > 0.0f)
		{

			//
			// Spawn new particles
			//

			spawnTimer += elapsedTime;

			while (spawnTimer >= spawnTime)
			{

				// Add a new particle if we have no dead ones to replace
				if (numParticles == particles.size())
					particles.push_back(Particle());

				// Emit the particle
				Emit(&particles[numParticles]);

				// Update particle counter
				numParticles ++;

				spawnTimer -= spawnTime;

			}

			// Update system life
			life -= elapsedTime;

		}

	}

}

void ParticleSystem::Render(sf::RenderWindow& renderer)
{

	//
	// Render all particles
	//

	for (int i = 0; i < numParticles; ++ i)
	{

		if (particles[i].life > 0.0f)
			renderer.Draw(particles[i].sprite);

	}

}

// Emits a particle
void ParticleSystem::Emit(Particle* particle)
{

	particle->position = position;

	particle->velocity = Vec2f(Random::RandomFloat(-1.0f, 1.0f), Random::RandomFloat(-1.0f, 1.0f)).Normalize() * 100.0f;

	particle->life = 1.0f;
	particle->sprite.SetImage(imageResource->Image());
	particle->sprite.SetBlendMode(sf::Blend::Add);

}

// Updates a particle
void ParticleSystem::UpdateParticle(Particle* particle, float elapsedTime)
{

	particle->life -= elapsedTime;

	particle->position += particle->velocity * elapsedTime;
	particle->UpdateSprite();

}

int ParticleSystem::GetNumParticles() const
{

	return numParticles;

}

bool ParticleSystem::IsAlive() const
{

	return (life > 0.0f) || (numParticles > 0);

}