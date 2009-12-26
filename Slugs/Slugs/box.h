//---------------------------------------------------------------
//
// Slugs
// box.h
//
//---------------------------------------------------------------

#pragma once

#include "vec2.h"

/*
	class Box
	A 2d bounding box
*/

class Box
{

public:

	Vec2f center;											// Center location of the box
	Vec2f extents;											// Half size of the box

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

	// Gets an expanded bounding box
	Box Expand(float x, float y) const;

};