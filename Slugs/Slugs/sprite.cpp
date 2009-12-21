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

void Sprite::SetPosition(Vector2& newPosition)
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

	data.SetImage(resource->Image());

}

void Sprite::SetPosition(float x, float y)
{

	data.SetPosition(x, y);

}

void Sprite::SetScale(float x, float y)
{

	data.SetScale(x, y);

}

Vector2 Sprite::GetPosition() const
{

	return Vector2(data.GetPosition());

}

Vector2 Sprite::GetSize() const
{

	return Vector2(data.GetSize());

}

Vector2 Sprite::GetScale() const
{

	return Vector2(data.GetScale());

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

	Vector2 size = data.GetSize();
	return size.x;

}

float Sprite::GetHeight() const
{

	Vector2 size = data.GetSize();
	return size.y;

}