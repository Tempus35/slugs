#pragma once

#include <sfml/graphics.hpp>

#include "global.h"
#include "singleton.h"
#include "color.h"
#include "sprite.h"

/*
	Forward Declarations
*/

class Camera;

/*
	class Renderer
*/

class Renderer : public Singleton<Renderer>
{

	friend class Singleton<Renderer>;

private:

	sf::RenderWindow				window;

	std::vector<sf::Shape>			debugShapes;

private:

	Renderer();
	~Renderer();

public:

	void Initialize(int width, int height, char* name);

	void Clear(Color& color);
	void Present();

	void Render(const Sprite& sprite);

	sf::RenderWindow& GetWindow();
	void SetIcon(sf::Image& icon);
	void SetCamera(Camera* camera);

	// Draws a debug circle
	void DrawCircle(const Vec2f& center, float radius, const Color& color);
	void DrawRay(const Vec2f& start, const Vec2f& direction, float length, const Color& color);

	// Draws all queued debug shapes
	void DebugDraw();

};