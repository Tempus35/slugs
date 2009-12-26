#include "fontresource.h"

FontResource::FontResource(const std::string& path, unsigned int size)
{

	font.LoadFromFile(path, size);

}

const sf::Font& FontResource::GetFont()
{

	return font;

}