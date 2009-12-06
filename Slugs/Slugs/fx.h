#pragma once

#include <SFML/graphics.hpp>

#include "particlesystem.h"
#include "imageresource.h"
#include "random.h"
#include "world.h"

#define SNOWSYSTEM_MAX_SNOWFLAKES	400
#define SNOWSYSTEM_SPEED_MIN		50
#define SNOWSYSTEM_SPEED_MAX		100
#define SNOWSYSTEM_SPAWN_TIME		1.0f / 10

class SnowSystem : public ParticleSystem
{

private:

	float spawnTime;
	int nextParticle;
	ImageResource* imageResource;
	sf::Rect<int> spawnArea;

public:

	SnowSystem(ImageResource* image, float density);

	void Update(float elapsedTime);

};