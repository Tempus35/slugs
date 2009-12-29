#pragma once

#include "debug.h"
#include "imageresource.h"
#include "vec2.h"
#include "color.h"
#include "fastmath.h"

class Sprite
{

protected:

	sf::Sprite data;
	Color color;

public:

	Sprite();
	Sprite(ImageResource* res, Color& color = Color(255, 255, 255));
	~Sprite();
	
	const sf::Sprite& GetData() const;

	//
	// Accessors
	//

	void SetPosition(Vec2f& newPosition);
	void SetColor(Color& newColor);
	void SetImage(ImageResource* resource);
	void SetCenter(float x, float y);
	void Center();
	void SetPosition(float x, float y);
	void SetRotation(float degrees);
	void SetOrientation(const Vec2f& direction);
	void SetScale(float x, float y);
	Vec2f GetPosition() const;
	Vec2f GetSize() const;
	Vec2f GetScale() const;
	void Move(float x, float y);
	void SetAlphaBlend(bool state);
	float GetWidth() const;
	float GetHeight() const;
	void SetMirrored(bool state);

};