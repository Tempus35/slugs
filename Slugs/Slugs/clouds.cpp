#include "clouds.h"

/*
	class Clouds
*/

//
// Initialization
//

Clouds::Clouds(ImageResource* cloudImage, Color& cloudColor, int x, int y, int terrainWidth, int numClouds, int cloudLayers)
{

	width = terrainWidth;
	speed = 30.0f;
	Sprite s;
	s.SetImage(cloudImage);
	s.SetColor(cloudColor);

	const int seperation = 50;
	float scale;
	for (int i = 0; i < numClouds; ++ i)
	{

		scale = Random::RandomFloat(0.5f, 1.5f);
		s.SetPosition((float)Random::RandomInt(0, terrainWidth), (float)(y + seperation * Random::RandomInt(0, cloudLayers)));
		s.SetScale(scale, scale);
		sprites.insert(sprites.end(), s);

	}

}

Clouds::~Clouds()
{

}

//
// Simulation
//

void Clouds::Update(float elapsedTime)
{

	Vec2f position, size, scale;

	for (unsigned int i = 0; i < sprites.size(); ++ i)
	{

		position = sprites[i].GetPosition();
		size = sprites[i].GetSize();
		scale = sprites[i].GetScale();
		
		sprites[i].Move(elapsedTime * speed * scale.x, 0.0f);

		if (position.x - size.x > width)
			sprites[i].SetPosition(-size.x, position.y);

	}

}

void Clouds::SetColor(Color newColor)
{

	for (unsigned int i = 0; i < sprites.size(); ++ i)
		sprites[i].SetColor(newColor);

}

void Clouds::SetWindSpeed(float newSpeed)
{

	speed = newSpeed;

}

//
// Rendering
//

void Clouds::Render()
{

	Renderer* renderer = Renderer::Get();

	for (unsigned int i = 0; i < sprites.size(); ++ i)
		renderer->Render(sprites[i]);

}