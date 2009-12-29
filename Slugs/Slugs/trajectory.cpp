#include "trajectory.h"

bool CalculateLaunchDirection(const Vec2f& from, const Vec2f& to, float speed, float absGravity, Vec2f& direction, bool chooseHigh)
{

	float x = to.x - from.x;
	float y = to.y - from.y;
	float speed2 = Sqr(speed);

	float d = speed2 - 2.0f * absGravity * (y + 0.5f * absGravity * (Sqr(x) / speed2));

	if (d < 0.0f)
	{

		// Out of range
		return false;

	}

	float sqrtD = Sqrt(d);
	float gxOverSpeed = absGravity * x / speed;

	float angle0 = Atan((speed + sqrtD) / gxOverSpeed);
	float angle1 = Atan((speed - sqrtD) / gxOverSpeed);

	if (from.x > to.x)
	{

		angle0 = Math::PI + angle0;
		angle1 = Math::PI + angle1;

	}

	float optimalAngle;
	
	if (chooseHigh)
		optimalAngle = Max(angle0, angle1);
	else
		optimalAngle = Min(angle0, angle1);

	direction = Vec2f(Cos(optimalAngle), Sin(optimalAngle));

	return true;

}