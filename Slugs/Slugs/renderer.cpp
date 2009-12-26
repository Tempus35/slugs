#include "renderer.h"
#include "camera.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Initialize(int width, int height, char* name, bool windowed)
{

	resolution.x = width;
	resolution.y = height;

	fullscreen = !windowed;

	windowTitle = name;

	UpdateWindow();

}

void Renderer::UpdateWindow()
{

	window.Close();

	unsigned long style = sf::Style::Close;

	if (fullscreen)
		style |= sf::Style::Fullscreen;

	window.Create(sf::VideoMode(resolution.x, resolution.y, 32), windowTitle, style);
	window.UseVerticalSync(true);
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

	if (camera)
		window.SetView(camera->GetView());
	else
		window.SetView(window.GetDefaultView());

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

void Renderer::RenderText(float x, float y, FontResource* fontResource, const std::string& text, float size, const Color& color, unsigned int flags)
{

	ASSERT(fontResource);

	sf::String outString(text, fontResource->GetFont(), size);

	if (flags & FontFlag_Centered)
	{
		sf::FloatRect bounds = outString.GetRect();
		outString.SetPosition(x - bounds.GetWidth() / 2, y - bounds.GetHeight() / 2);
	}
	else
		outString.SetPosition(x, y);

	outString.SetColor(color.ToSF());

	unsigned long style = 0;

	if (flags & FontFlag_Bold)
		style |= sf::String::Bold;

	outString.SetStyle(style);

	window.Draw(outString);

}

void Renderer::RenderTextShadowed(float x, float y, FontResource* fontResource, const std::string& text, float size, const Color& color, const Color& shadow, unsigned int flags)
{

	const float spacing = 2.0f;

	RenderText(x + spacing, y + spacing, fontResource, text, size, shadow, flags);
	RenderText(x, y, fontResource, text, size, color, flags);

}

void Renderer::SetFullscreen(bool state)
{

	if (fullscreen != state)
	{

		fullscreen = state;
		UpdateWindow();

	}

}

bool Renderer::IsFullscreen() const
{

	return Renderer::fullscreen;

}

int Renderer::GetWidth() const
{

	return resolution.x;

}

int Renderer::GetHeight() const
{

	return resolution.y;

}
