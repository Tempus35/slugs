#include "sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(ImageResource* resource, Color& hueColor)
{

	data.SetImage(resource->Image());

	color = hueColor;
	data.SetColor(color.ToSF());

}

Sprite::~Sprite()
{

}

const sf::Sprite& Sprite::GetData() const
{

	return data;

}

//
// Accessors
//

void Sprite::SetPosition(Vec2f& newPosition)
{

	data.SetPosition(newPosition.ToSF());

}

void Sprite::SetColor(Color& newColor)
{

	color = newColor;
	data.SetColor(color.ToSF());

}

void Sprite::SetImage(ImageResource* resource)
{

	ASSERT(resource);

	data.SetImage(resource->Image());

}

void Sprite::SetCenter(float x, float y)
{

	data.SetCenter(x, y);

}

void Sprite::Center()
{

	data.SetCenter(data.GetSize().x / 2, data.GetSize().y / 2);

}

void Sprite::SetPosition(float x, float y)
{

	data.SetPosition(x, -y);

}

void Sprite::SetRotation(float degrees)
{

	data.SetRotation(degrees);

}

void Sprite::SetOrientation(const Vec2f& direction)
{

	float angle = atan2f(direction.y, direction.x);

	if (angle < 0)
		angle += Math::TWO_PI;

   	data.SetRotation(Degrees(angle));

}

void Sprite::SetScale(float x, float y)
{

	data.SetScale(x, y);

}

Vec2f Sprite::GetPosition() const
{

	return Vec2f(data.GetPosition());

}

Vec2f Sprite::GetSize() const
{

	return Vec2f(data.GetSize());

}

Vec2f Sprite::GetScale() const
{

	return Vec2f(data.GetScale());

}

void Sprite::Move(float x, float y)
{

	data.Move(x, y);

}

void Sprite::SetAlphaBlend(bool state)
{

	if (state)
		data.SetBlendMode(sf::Blend::Alpha);
	else
		data.SetBlendMode(sf::Blend::None);

}

float Sprite::GetWidth() const
{

	Vec2f size = data.GetSize();
	return size.x;

}

float Sprite::GetHeight() const
{

	Vec2f size = data.GetSize();
	return size.y;

}

void Sprite::SetMirrored(bool state)
{

	data.FlipX(state);

}