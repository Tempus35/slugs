#pragma once

#include <vector>
using namespace std;

#include <sfml/graphics.hpp>

#include "vector2.h"
#include "resources.h"
#include "random.h"
#include "fastmath.h"
#include "renderer.h"

/*
	class Water
*/

class Water
{

private:

	ImageResource* image;
	vector<Sprite> sprites;
	Color color;
	int width;
	float speed;
	Vector2 position;
	int sectionWidth;
	float offset;

public:

	//
	// Initialization
	//

	Water(ImageResource* waterImage, Color& waterColor, int x, int y, int totalWidth, float movementSpeed);
	~Water();

	//
	// Simulation
	//

	void Update(float elapsedTime);
	void SetColor(Color& newColor);

	//
	// Rendering
	//

	void Render();

};