#pragma once

#include <vector>
#include <sfml/graphics.hpp>

#include "resource.h"
#include "random.h"
#include "renderer.h"

using namespace std;

/*
	class Clouds
*/

class Clouds
{

private:

	vector<Sprite> sprites;
	float speed;
	int width;

public:

	//
	// Initialization
	//

	Clouds(ImageResource* cloudImage, Color& cloudColor, int x, int y, int terrainWidth, int numClouds, int cloudLayers);
	~Clouds();

	//
	// Simulation
	//

	void Update(float elapsedTime);
	void SetColor(Color newColor);
	void SetWindSpeed(float newSpeed);

	//
	// Rendering
	//

	void Render();

};
