#include "circle.h"

Circle::Circle()
{

	center = Vec2f(0.0f, 0.0f);
	radius = 0.0f;

}

Circle::Circle(const Vec2f& _center, float _radius)
{

	center = _center;
	radius = _radius;

}

bool Circle::Contains(const Vec2f& point) const
{

	Vec2f delta = point - center;
	
	if (delta.LengthSquared() <= Sqr(radius))
		return true;

	return false;

}