#include "shape.h"

/*
	class Shape
*/

Shape::Shape(bool filled, bool outlined)
{

	SetFilled(filled);
	SetOutlined(outlined);

}

const sf::Drawable& Shape::GetData() const
{

	return shape;

}

void Shape::SetPosition(const Vec2f& position)
{

	shape.SetPosition(position.x, position.y);

}

void Shape::SetFilled(bool state)
{

	shape.EnableFill(state);

}

void Shape::SetFillColor(const Color& color)
{

	shape.SetColor(color.ToSF());

}

void Shape::SetOutlined(bool state)
{

	shape.EnableOutline(state);

}

void Shape::SetOutlineColor(const Color& color)
{

	shape.SetColor(color.ToSF());

}

void Shape::CreateBox(const Vec2f& position, const Vec2f& size, const Color& color)
{

	shape = sf::Shape::Rectangle(position.x, position.y, position.x + size.x, position.y + size.y, color.ToSF());

}

/*
	class Pie
*/

Shape_Pie::Shape_Pie(float pieRadius, float startAngle, float amountFilled) : Shape(true, false)
{

	radius = pieRadius;
	offset = startAngle;
	fill = amountFilled;

	Update();

}

void Shape_Pie::Update()
{

	sf::Vector2f p = shape.GetPosition();
	shape = sf::Shape();
	shape.SetPosition(p);
	shape.SetBlendMode(sf::Blend::Alpha);
	
	// Add mid point
	shape.AddPoint(0.0f, 0.0f, Color(40, 128, 40, 0).ToSF());

	// Add outer points
	const float STEP_SIZE = Radians(10.0f);

	float angle = fill;
	float done = 0.0f;
	float size = Min(fill, Math::TWO_PI);
	while ((angle > STEP_SIZE) && (done < size))
	{

		shape.AddPoint(Cos(angle) * radius, Sin(angle) * radius, Color(40, 128, 64, (byte)(64.0f * (1.0f - (done / size)))).ToSF());

		done += STEP_SIZE;
		angle -= STEP_SIZE;

	}

}

void Shape_Pie::AdjustFill(float amount)
{

	fill += amount;

	//if (fill > Math::TWO_PI)
	//	fill -= Math::TWO_PI;

	Update();

}