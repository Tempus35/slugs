#include "renderer.h"
#include "camera.h"

Renderer::Renderer()
{

	defaultFont = NULL;

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

void Renderer::DrawDebugCircle(const Vec2f& center, float radius, const Color& color)
{

	DebugShape* d = new DebugShape();
	
	d->shape = sf::Shape::Circle(center.x, -center.y, radius, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableOutline(true);
	d->shape.EnableFill(false);

	debugLayer.push_back(d);

}

void Renderer::DrawDebugRay(const Vec2f& start, const Vec2f& direction, float length, const Color& color)
{

	DebugShape* d = new DebugShape();

	d->shape = sf::Shape::Line(start.x, -start.y, start.x + direction.x * length, -(start.y + direction.y * length), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);

	debugLayer.push_back(d);

}

void Renderer::DrawDebugLine(const Vec2f& start, const Vec2f& end, const Color& color)
{

	DebugShape* d = new DebugShape();

	d->shape = sf::Shape::Line(start.x, -start.y, end.x, -end.y, 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);

	debugLayer.push_back(d);

}


void Renderer::DrawDebugArrow(const Vec2f& start, const Vec2f& end, const Color& color)
{

	DebugShape* d;

	d = new DebugShape();
	d->shape = sf::Shape::Line(start.x, -start.y, end.x, -end.y, 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

	Vec2f direction = (end - start).Normalize();
	Vec2f cross = direction.Perpendicular();

	const float head = 10.0f;
	Vec2f headPoint = end - direction * head;

	d = new DebugShape();
	d->shape = sf::Shape::Line(end.x, -end.y, headPoint.x + cross.x * head, -(headPoint.y + cross.y * head), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

	d = new DebugShape();
	d->shape = sf::Shape::Line(end.x, -end.y, headPoint.x - cross.x * head, -(headPoint.y - cross.y * head), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

}


void Renderer::DrawDebugTrajectory(const Vec2f& start, const Vec2f& direction, float initialSpeed, const Color& color)
{

	const float sample = 0.025f;
	const float g = -1000.0f;

	Vec2f previous, current;
	previous = start;

	float t = sample;
	Vec2f velocity = direction * initialSpeed;

	while (previous.y >= 0.0f)
	{

		current.x = start.x + velocity.x * t;
		current.y = start.y + velocity.y * t + 0.5f * g * t * t;

		DebugShape* d = new DebugShape();

		d->shape = sf::Shape::Line(current.x, -current.y, previous.x, -previous.y, 1.0f, color.ToSF(), 1.0f, color.ToSF());
		d->shape.EnableFill(true);
		d->shape.EnableOutline(false);

		debugLayer.push_back(d);

		previous = current;

		t += sample;

	}

}

void Renderer::DrawDebugHint(const Vec2f& position, const std::string& text, const Color& color)
{

	const float border = 5.0f;

	DebugString* s = new DebugString();

	s->string = sf::String(text, sf::Font::GetDefaultFont(), 16.0f);
	s->string.SetPosition(position.x, -position.y);
	s->string.SetColor(color.ToSF());

	DebugShape* d = new DebugShape();

	const sf::FloatRect& hintRect = s->string.GetRect();
	d->shape = sf::Shape::Rectangle(hintRect.Left - border, hintRect.Bottom + border, hintRect.Right + border, hintRect.Top - border, Color(0, 0, 0, 128).ToSF());

	debugLayer.push_back(d);
	debugLayer.push_back(s);

}

void Renderer::DrawDebugBox(const Boxf& box, const Color& color)
{

	DebugShape* d = new DebugShape();

	d->shape = sf::Shape::Rectangle(box.center.x - box.extents.x, -(box.center.y - box.extents.y), box.center.x + box.extents.x, -(box.center.y + box.extents.y), color.ToSF());
	debugLayer.push_back(d);

}

void Renderer::DebugDraw()
{

	for (unsigned int i = 0; i < debugLayer.size(); ++ i)
	{

		window.Draw(debugLayer[i]->GetDrawable());
		delete debugLayer[i];

	}

	debugLayer.clear();

}

void Renderer::SetDefaultFont(FontResource* font)
{

	defaultFont = font;

}

void Renderer::RenderText(float x, float y, FontResource* fontResource, const std::string& text, float size, const Color& color, unsigned int flags)
{

	FontResource* font;

	if (fontResource == NULL)
	{

		ASSERT(defaultFont != NULL);
		font = defaultFont;

	}
	else
		font = fontResource;

	sf::String outString(text, font->GetFont(), size);

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
