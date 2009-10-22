#pragma once

#include "resources.h"
#include "vector2.h"
#include "color.h"

class Sprite
{

protected:

	sf::Sprite data;
	Color color;

public:

	Sprite();
	Sprite(ImageResource* res, Color& color = Color(255, 255, 255));
	~Sprite();
	
	sf::Sprite& GetData();

	//
	// Accessors
	//

	void SetPosition(Vector2& newPosition);
	void SetColor(Color& newColor);
	void SetImage(ImageResource* resource);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	Vector2 GetPosition();
	Vector2 GetSize();
	Vector2 GetScale();
	void Move(float x, float y);
	void SetAlphaBlend(bool state);
	float GetWidth();
	float GetHeight();

};