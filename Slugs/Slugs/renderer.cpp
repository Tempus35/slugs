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

void Renderer::Clear(const Color& color)
{

	window.Clear(color.ToSF());

}

void Renderer::Present()
{

	window.Display();

}

void Renderer::Render(const Renderable& renderable)
{

	window.Draw(renderable.GetData());

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

	Shape* d = new Shape();
	
	d->shape = sf::Shape::Circle(center.x, -center.y, radius, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableOutline(true);
	d->shape.EnableFill(false);

	debugLayer.push_back(d);

}

void Renderer::DrawDebugRay(const Vec2f& start, const Vec2f& direction, float length, const Color& color)
{

	Shape* d = new Shape();

	d->shape = sf::Shape::Line(start.x, -start.y, start.x + direction.x * length, -(start.y + direction.y * length), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);

	debugLayer.push_back(d);

}

void Renderer::DrawDebugLine(const Vec2f& start, const Vec2f& end, const Color& color)
{

	Shape* d = new Shape();

	d->shape = sf::Shape::Line(start.x, -start.y, end.x, -end.y, 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);

	debugLayer.push_back(d);

}


void Renderer::DrawDebugArrow(const Vec2f& start, const Vec2f& end, const Color& color)
{

	Shape* d;

	d = new Shape();
	d->shape = sf::Shape::Line(start.x, -start.y, end.x, -end.y, 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

	Vec2f direction = (end - start).Normalize();
	Vec2f cross = direction.Perpendicular();

	const float head = 10.0f;
	Vec2f headPoint = end - direction * head;

	d = new Shape();
	d->shape = sf::Shape::Line(end.x, -end.y, headPoint.x + cross.x * head, -(headPoint.y + cross.y * head), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

	d = new Shape();
	d->shape = sf::Shape::Line(end.x, -end.y, headPoint.x - cross.x * head, -(headPoint.y - cross.y * head), 1.0f, color.ToSF(), 1.0f, color.ToSF());
	d->shape.EnableFill(true);
	d->shape.EnableOutline(false);
	debugLayer.push_back(d);

}


void Renderer::DrawDebugTrajectory(const Vec2f& start, const Vec2f& direction, float initialSpeed, float absGravity, const Color& color)
{

	const float sample = 0.025f;
	const float g = -absGravity;

	Vec2f previous, current;
	previous = start;

	float t = sample;
	Vec2f velocity = direction * initialSpeed;

	while (previous.y >= 0.0f)
	{

		current.x = start.x + velocity.x * t;
		current.y = start.y + velocity.y * t + 0.5f * g * t * t;

		Shape* d = new Shape();

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

	Text* s = new Text();

	s->string = sf::String(text, sf::Font::GetDefaultFont(), 16.0f);
	s->string.SetPosition(position.x, -position.y);
	s->string.SetColor(color.ToSF());

	Shape* d = new Shape();

	const sf::FloatRect& hintRect = s->string.GetRect();
	d->shape = sf::Shape::Rectangle(hintRect.Left - border, hintRect.Bottom + border, hintRect.Right + border, hintRect.Top - border, Color(0, 0, 0, 128).ToSF());

	debugLayer.push_back(d);
	debugLayer.push_back(s);

}

void Renderer::DrawDebugBox(const Boxf& box, const Color& color)
{

	Shape* d = new Shape();

	d->shape = sf::Shape::Rectangle(box.center.x - box.extents.x, -(box.center.y - box.extents.y), box.center.x + box.extents.x, -(box.center.y + box.extents.y), color.ToSF());
	debugLayer.push_back(d);

}

void Renderer::DebugDraw()
{

	for (unsigned int i = 0; i < debugLayer.size(); ++ i)
	{

		window.Draw(debugLayer[i]->GetData());
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

Boxf Renderer::MeasureText(const std::string& text, FontResource* fontResource, float size)
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
	sf::FloatRect bounds = outString.GetRect();

	Vec2f extents = Vec2f(bounds.GetWidth() * 0.5f, bounds.GetHeight() * 0.5f);
	return Boxf(Vec2f(bounds.Left + extents.x, bounds.Top + extents.y), extents);

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

Vec2i Renderer::GetRelativeCoordinate(int x, int y, RenderPosition position) const
{

	switch (position)
	{

	case RenderPosition_Center:
		return Vec2i(resolution.x / 2 + x, resolution.y / 2 + y);

	case RenderPosition_TopLeft:
		return Vec2i(x, y);

	case RenderPosition_TopRight:
		return Vec2i(resolution.x - x, y);

	case RenderPosition_BottomLeft:
		return Vec2i(x, resolution.y - y);

	case RenderPosition_BottomRight:
		return Vec2i(resolution.x - x, resolution.y - y);

	case RenderPosition_Left:
		return Vec2i(x, resolution.y / 2);

	case RenderPosition_Right:
		return Vec2i(resolution.x - x, resolution.y / 2);

	case RenderPosition_Top:
		return Vec2i(resolution.x / 2, y);

	case RenderPosition_Bottom:
		return Vec2i(resolution.x / 2, resolution.y - y);

	}

	return Vec2i(x, y);

}
