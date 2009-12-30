#include "fontresource.h"

FontResource::FontResource(const std::string& path, unsigned int size) : Resource(ResourceType_Font)
{

	font.LoadFromFile(path, size);

}

const sf::Font& FontResource::GetFont()
{

	return font;

}