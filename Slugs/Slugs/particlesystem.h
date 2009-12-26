#pragma once

#include "fxmanager.h"
#include "particle.h"
#include "utility.h"
#include "random.h"

class ParticleSystem : public Effect
{

protected:

	ImageResource*				imageResource;
	std::vector<Particle>		particles;
	float						spawnTimer;
	float						spawnTime;
	int							numParticles;

public:

	// Constructor
	ParticleSystem(ImageResource* _imageResource, float _life);

	// Destructor
	~ParticleSystem();

	// Updates the particle system
	virtual void Update(float elapsedTime);

	// Renders the particle system
	virtual void Render(sf::RenderWindow& renderer);

	// Emits a particle
	virtual void Emit(Particle* particle);

	// Updates a particle
	virtual void UpdateParticle(Particle* particle, float elapsedTime);

	// Gets the current number of particles active in the system
	virtual int GetNumParticles() const;

	// Returns true if the particle system should be rendered
	virtual bool IsAlive() const;

};