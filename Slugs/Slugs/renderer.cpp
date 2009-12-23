#include "renderer.h"
#include "camera.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Initialize(int width, int height, char* name)
{

	window.Create(sf::VideoMode(width, height, 32), name);
	window.UseVerticalSync(false);
	window.SetFramerateLimit(60);
	window.EnableKeyRepeat(false);

}

void Renderer::Clear(Color& color)
{

	window.Clear(color.ToSF());

}

void Renderer::Present()
{

	window.Display();

}

void Renderer::Render(const Sprite& sprite)
{

	window.Draw(sprite.GetData());

}

sf::RenderWindow& Renderer::GetWindow()
{

	return window;

}

void Renderer::SetIcon(sf::Image& image)
{

	window.SetIcon(image.GetWidth(), image.GetHeight(), image.GetPixelsPtr());

}

void Renderer::SetCamera(Camera* camera)
{

	window.SetView(camera->GetView());

}

void Renderer::DrawCircle(const Vec2f& center, float radius, const Color& color)
{

	sf::Shape circle = sf::Shape::Circle(center.x, -center.y, radius, color.ToSF(), 1.0f, color.ToSF());
	circle.EnableOutline(true);
	circle.EnableFill(false);

	debugShapes.push_back(circle);

}

void Renderer::DrawRay(const Vec2f& start, const Vec2f& direction, float length, const Color& color)
{

	sf::Shape line = sf::Shape::Line(start.x, -start.y, start.x + direction.x * length, -(start.y + direction.y * length), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	line.EnableFill(false);
	line.EnableOutline(true);

	debugShapes.push_back(line);

}

void Renderer::DebugDraw()
{

	for (unsigned int i = 0; i < debugShapes.size(); ++ i)
		window.Draw(debugShapes[i]);

	debugShapes.clear();

}