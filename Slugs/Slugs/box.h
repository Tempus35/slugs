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

template <class T>
class Box
{

public:

	Vec2<T> center;											// Center location of the box
	Vec2<T> extents;										// Half size of the box

public:

	// Constructors
	Box();
	Box(const Vec2<T>& _center, const Vec2<T>& _extents);

	// Gets the minimum of the box
	Vec2<T> GetMin() const;

	// Gets the maximum of the box
	Vec2<T> GetMax() const;
	
	// Returns true if the box contains the given point
	bool Contains(const Vec2<T>& point) const;
	bool Contains(T x, T y) const;

	// Gets an expanded bounding box
	Box Expand(T x, T y) const;

};

// Predefined types
typedef Box<float>	Boxf;
typedef Box<int>	Boxi;

#include "box.inl"