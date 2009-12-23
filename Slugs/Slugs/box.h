#pragma once

#include "vec2.h"

class Box
{

public:

	Vec2f center;
	Vec2f extents;

public:

	// Constructors
	Box();
	Box(Vec2f _center, Vec2f _extents);

	// Gets the minimum of the box
	Vec2f GetMin() const;

	// Gets the maximum of the box
	Vec2f GetMax() const;
	
	// Returns true if the box contains the given point
	bool Contains(const Vec2f& point) const;
	bool Contains(float x, float y) const;

};