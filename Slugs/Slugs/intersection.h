#pragma once

#include "fastmath.h"
#include "vec2.h"
#include "box.h"

/*
	Forward declarations
*/

class Object;

/*
	Enumeration of intersection types
*/

enum IntersectionType
{

	IntersectionType_None,				// No intersection occurred
	IntersectionType_Terrain,			// An intersection with the terrain
	IntersectionType_Object,			// An intersection with an object

};

/*
	struct Intersection
	Holds information about an intersection with the world
*/

struct Intersection
{

	IntersectionType	type;			// Type of the intersection
	Vec2f				position;		// Location of the intersection
	float				distance;		// Distance to the intersection
	Object*				object;			// Object hit, or NULL if no collision with an object

	Intersection();

};

/*
	Geometric Intersection Tests
*/

bool RayCircleIntersection(const Vec2f& rayStart, const Vec2f& rayDirection, const Vec2f& circleCenter, float circleRadius, float maxRange, Vec2f& intersection);
bool BoxBoxIntersection(const Boxf& a, const Boxf& b);