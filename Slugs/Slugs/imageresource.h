#pragma once

#include <sfml/graphics.hpp>

#include "resource.h"

/*
	class ImageResource
	Container for SFML Image
*/

class ImageResource : public Resource
{

private:

	sf::Image image;

public:

	//
	// Initialization
	//

	ImageResource(int width, int height, bool smooth);
	ImageResource(char* path);
	~ImageResource();

	//
	// Accessors
	//

	sf::Image& Image();

};