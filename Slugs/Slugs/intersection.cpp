#include "intersection.h"
#include "renderer.h"

bool RayCircleIntersection(const Vec2f& rayStart, const Vec2f& rayDirection, const Vec2f& circleCenter, float circleRadius, float maxRange, Vec2f& intersection)
{

	Vec2f circleToRay = circleCenter - rayStart;
	float d = DotProduct(rayDirection, circleToRay);

	if ((d > maxRange) || (d < 0.0f))
		return false;

	Vec2f pointOnRay = rayStart + rayDirection * d;
	float t = (pointOnRay - circleCenter).Length();

	if (t > circleRadius)
		return false;
	else if (t == circleRadius)
	{

		// One intersection
		intersection = pointOnRay;
		return true;

	}
	else
	{

		// Two intersections, return the one nearest to the ray start
		float x = Sqrt(circleRadius * circleRadius - t * t);
		intersection = pointOnRay - rayDirection * x;

		return true;

	}
	
}