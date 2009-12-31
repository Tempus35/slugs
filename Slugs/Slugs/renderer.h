#pragma once

#include <sfml/graphics.hpp>

#include "global.h"
#include "singleton.h"
#include "renderable.h"
#include "color.h"
#include "sprite.h"
#include "fontresource.h"
#include "box.h"
#include "shape.h"
#include "text.h"

/*
	Enumeration of font rendering styles (can be ORed together)
*/

enum FontFlag
{

	FontFlag_None = 0,
	FontFlag_Bold = 1,
	FontFlag_Centered = 2,

};

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
	Vec2i							resolution;
	bool							fullscreen;
	std::string						windowTitle;

	std::vector<Renderable*>		debugLayer;

	FontResource*					defaultFont;

private:

	Renderer();
	~Renderer();

public:

	void Initialize(int width, int height, char* name, bool windowed = true);
	void UpdateWindow();

	void Clear(const Color& color);
	void Present();

	void Render(const Renderable& renderable);

	sf::RenderWindow& GetWindow();
	void SetIcon(sf::Image& icon);
	void SetCamera(Camera* camera);

	// Draws a debug circle
	void DrawDebugCircle(const Vec2f& center, float radius, const Color& color);

	// Draws a debug ray
	void DrawDebugRay(const Vec2f& start, const Vec2f& direction, float length, const Color& color);

	// Draws a debug line
	void DrawDebugLine(const Vec2f& start, const Vec2f& end, const Color& color);

	// Draws a debug arrow
	void DrawDebugArrow(const Vec2f& start, const Vec2f& end, const Color& color);

	// Draws a debug trajectory
	void DrawDebugTrajectory(const Vec2f& start, const Vec2f& direction, float initialSpeed, float absGravity, const Color& color);

	// Draws a debug hint box
	void DrawDebugHint(const Vec2f& position, const std::string& text, const Color& color);

	// Draws a debug box
	void DrawDebugBox(const Boxf& box, const Color& color);

	// Draws all queued debug shapes
	void DebugDraw();

	// Sets the default font
	void SetDefaultFont(FontResource* font);

	// Render text to the screen
	void RenderText(float x, float y, FontResource* fontResource, const std::string& text, float size = 30.0f, const Color& color = Color(255, 255, 255), unsigned int flags = FontFlag_None);

	// Measures a text string
	Boxf MeasureText(const std::string& text, FontResource* fontResource, float size);

	// Render text to the screen with a shadow effect
	void RenderTextShadowed(float x, float y, FontResource* fontResource, const std::string& text, float size = 30.0f, const Color& color = Color(255, 255, 255), const Color& shadow = Color(0, 0, 0), unsigned int flags = FontFlag_None);

	// Enables or disables fullscreen mode
	void SetFullscreen(bool state);

	// Returns true if we are rendering in full screen mode
	bool IsFullscreen() const;

	// Gets the horizontal resolution
	int GetWidth() const;

	// Gets the vertical resolution
	int GetHeight() const;

};