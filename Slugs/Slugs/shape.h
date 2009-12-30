#pragma once

#include "fastmath.h"
#include "renderable.h"
#include "color.h"
#include "vec2.h"

/*
	class Shape
	Shape renderable used by Renderer
*/

class Shape : public Renderable
{

friend class Renderer;

protected:

	sf::Shape shape;

public:

	// Constructor
	Shape(bool filled = true, bool outlined = true);

	// Gets the sf::Drawable
	virtual const sf::Drawable& GetData() const;

	// Sets the position of the shape
	virtual void SetPosition(const Vec2f& position);

	// Sets if the shape is filled
	virtual void SetFilled(bool state);

	// Sets the fill color
	virtual void SetFillColor(const Color& color);

	// Sets if the shappe has an outline
	virtual void SetOutlined(bool state);

	// Sets the outline color
	virtual void SetOutlineColor(const Color& color);

	// Sets the shape up as a box
	void CreateBox(const Vec2f& position, const Vec2f& size, const Color& color);

};

/*
	class Pie
	A pie shape
*/

class Shape_Pie : public Shape
{

protected:

	float offset;										// Offset of the start of the fill in radians
	float fill;											// Amount filled in radians
	float radius;

protected:

	// Updates the internal shape
	void Update();

public:

	// Constructor
	Shape_Pie(float pieRadius = 110.0f, float startAngle = 0.0f, float amountFilled = 0.0f);

	// Adjust the amount of the pie that is filled
	void AdjustFill(float amount);

};