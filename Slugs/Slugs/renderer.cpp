#include "renderer.h"

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

void Renderer::SetView(sf::View& view)
{

	window.SetView(view);

}