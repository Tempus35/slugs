#include "fx.h"
#include "game.h"

SnowSystem::SnowSystem(ImageResource* image, float density)
{

	if (density <= 0.0f)
		density = 1.0f;

	maxParticles = (int)(density * SNOWSYSTEM_MAX_SNOWFLAKES);
	particles = new Particle[maxParticles];

	imageResource = image;

	World* world = Game::Get()->GetWorld();
	spawnArea = sf::Rect<int>(0, -world->HeightInPixels() - 50, world->WidthInPixels(), -world->HeightInPixels() - 50); 

	continuous = true;

	spawnTime = 0.0f;
	nextParticle = 0;

}

void SnowSystem::Update(float elapsedTime)
{

	while (spawnTime > SNOWSYSTEM_SPAWN_TIME)
	{

		particles[nextParticle].SetPosition(Random::RandomInt(spawnArea.Left, spawnArea.Right), Random::RandomInt(spawnArea.Bottom, spawnArea.Top));
		particles[nextParticle].SetImage(imageResource);
		particles[nextParticle].SetLife(10000.0f);
		particles[nextParticle].SetScale(Random::RandomFloat(0.5f, 1.0f));
		particles[nextParticle].SetVelocity(0.0f, Random::RandomFloat(SNOWSYSTEM_SPEED_MIN, SNOWSYSTEM_SPEED_MAX));

		nextParticle ++;
		if (nextParticle == maxParticles)
			nextParticle = 0;

		spawnTime -= SNOWSYSTEM_SPAWN_TIME;

	}

	ParticleSystem::Update(elapsedTime);

	spawnTime += elapsedTime;

}