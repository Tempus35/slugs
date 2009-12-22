#include "intersection.h"

bool RayCircleIntersection(const Vector2& rayStart, const Vector2& rayDirection, const Vector2& circleCenter, float circleRadius, float maxRange, Vector2& intersection)
{

	Vector2 circleToRay = circleCenter - rayStart;
	float d = VectorDot(rayDirection, circleToRay);

	if (d > maxRange)
		return false;

	Vector2 pointOnRay = rayStart + rayDirection * d;
	float t = VectorLength(pointOnRay - circleCenter);

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