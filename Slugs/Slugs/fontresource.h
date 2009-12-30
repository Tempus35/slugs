#pragma once

#include <sfml/graphics.hpp>

#include "debug.h"
#include "resource.h"

/*
	class FontResource
	Resource container for am SFML::Font object
*/

class FontResource : public Resource
{

protected:

	sf::Font		font;

public:

	// Constructor
	FontResource(const std::string& path, unsigned int size = 64);

	// Object accessor
	const sf::Font& GetFont();

};