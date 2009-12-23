#include "water.h"

/*
	class Water
*/

//
// Initilialization
//

Water::Water(ImageResource* waterImage, Color& waterColor, int x, int y, int totalWidth, float movementSpeed)
{

	if (waterImage)
	{

		image = waterImage;
		position = Vec2i(x, y);
		width = totalWidth;
		speed = movementSpeed;
		color = waterColor;

		sf::Image& im = image->Image();
		sectionWidth = im.GetWidth();
		int numSprites = width / sectionWidth + 3;

		offset = Random::RandomFloat(0, (float)sectionWidth);

		int cx = x - sectionWidth + (int)offset;
		Sprite s;
		s.SetImage(image);
		s.SetColor(color);			
		for (int i = 0; i < numSprites; ++ i)
		{

			s.SetPosition((float)cx, (float)y);
			sprites.insert(sprites.end(), s);

			cx += sectionWidth;

		}

	}

}

Water::~Water()
{

}

//
// Simulation
//

void Water::Update(float elapsedTime)
{

	offset += speed * elapsedTime;

	if (offset > sectionWidth)
		offset -= sectionWidth;
	else if (offset < -sectionWidth)
		offset += sectionWidth;

	int cx = RoundToInt(position.x - sectionWidth + offset);
	for (unsigned int i = 0; i < sprites.size(); ++ i)
	{

		sprites[i].SetPosition((float)cx, (float)position.y);
		cx += sectionWidth;

	}

}

void Water::SetColor(Color& newColor)
{

	color = newColor;

	for (unsigned int i = 0; i < sprites.size(); ++ i)
		sprites[i].SetColor(color);

}

//
// Rendering
//

void Water::Render()
{

	Renderer* renderer = Renderer::Get();

	for (unsigned int i = 0; i < sprites.size(); ++ i)
		renderer->Render(sprites[i]);

}