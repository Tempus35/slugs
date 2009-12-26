#pragma once

#include "fxmanager.h"
#include "particle.h"
#include "utility.h"

class ParticleSystem : public Effect
{

protected:

	ImageResource*				imageResource;
	std::vector<Particle>		particles;
	float						life;
	bool						alive;

public:

	ParticleSystem(ImageResource* _imageResource, float life);
	~ParticleSystem();

	virtual void Update(float elapsedTime);
	virtual void Render(sf::RenderWindow& renderer);

};