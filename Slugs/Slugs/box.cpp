//---------------------------------------------------------------
//
// Slugs
// box.cpp
//
//---------------------------------------------------------------

#include "box.h"

Box::Box()
{

	center = Vec2f(0.0f, 0.0f);
	extents = Vec2f(0.0f, 0.0f);

}

Box::Box(Vec2f _center, Vec2f _extents)
{

	center = _center;
	extents = _extents;

}

Vec2f Box::GetMin() const
{

	return center - extents;

}

Vec2f Box::GetMax() const
{

	return center + extents;

}

bool Box::Contains(const Vec2f& point) const
{

	return Contains(point.x, point.y);

}

bool Box::Contains(float x, float y) const
{

	Vec2f min = GetMin();
	Vec2f max = GetMax();

	if ((x >= min.x) && (x <= max.x) && (y >= min.y) && (y <= max.y))
		return true;

	return false;

}

Box Box::Expand(float x, float y) const
{

	return Box(center, Vec2f(extents.x + x, extents.y + y));

}