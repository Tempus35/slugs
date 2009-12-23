#pragma once

#include "vec2.h"
#include "fastmath.h"

/*
	class Circle
*/

class Circle
{

public:

	Vec2f		center;
	float		radius;

public:

	// Constructors
	Circle();
	Circle(const Vec2f& _center, float _radius);

	// Tests a point to see if it is inside the circle
	bool Contains(const Vec2f& point);

};