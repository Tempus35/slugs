#pragma once

#include <sfml/graphics.hpp>

/*
	class Renderable
	Base class for classes rendered using RenderManager
*/

class Renderable
{

friend class Renderer;

protected:

public:

	// Must be implemented by child classes to return their sfml drawable
	virtual const sf::Drawable& GetData() const = 0;

};