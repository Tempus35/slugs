#pragma once

#include <sfml/graphics.hpp>

#include "defs.h"
#include "singleton.h"
#include "color.h"
#include "sprite.h"

class Renderer : public Singleton<Renderer>
{

	friend class Singleton<Renderer>;

private:

	Renderer();
	~Renderer();

	sf::RenderWindow window;

public:

	void Initialize(int width, int height, char* name);

	void Clear(Color& color);
	void Present();

	void Render(const Sprite& sprite);

	sf::RenderWindow& GetWindow();
	void SetIcon(sf::Image& icon);
	void SetView(sf::View& view);

};