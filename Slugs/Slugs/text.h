#pragma once

#include "renderable.h"

/*
	class Text
	Text renderable used by Renderer
*/

class Text : public Renderable
{

friend class Renderer;

protected:

	sf::String string;

public:

	const sf::Drawable& GetData() const;

};