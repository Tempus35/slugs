#pragma once

#include <math.h>

/*
	class Math
	Container for mathmatical constants
*/

class Math
{

public:

	static const float PI;
	static const float TWO_PI;
	static const float PI_OVER_2;
	static const float PI_OVER_4;
	static const float PI_OVER_180;
	static const float ONE_OVER_PI;
	static const float EPSILON;
	static const float INFINITY;

};

// Absolute value
template <class T>
inline T Abs(T x)
{

	return x < 0 ? -x : x;
	
}

// Rounds a floating point value to the nearest int
inline int RoundToInt(float f)
{

	return (int)(f + 0.5f);

}

// Rounds a floating point value down to an int
inline int RoundDownToInt(float x)
{

	return (int)floorf(x);

}

// Rounds a floating point value up to an int
inline int RoundUpToInt(float x)
{

	return (int)ceilf(x);

}


// Rounds a floating point value down
inline float Floor(float x)
{

	return floorf(x);

}

// Rounds a floating point value up
inline float Ceil(float x)
{

	return ceilf(x);

}

// Rounds a floating point value
inline float Round(float f)
{

	return floorf(f + 0.5f);

}

// Calculates the sin of x
inline float Sin(float x)
{

	return sinf(x);
	
}

// Calculates the cosine of x
inline float Cos(float x)
{
	
	return cosf(x);
	
}

// Calculates the arccosine of x
inline float Acos(float x)
{

	return acosf(x);

}

// Calculates the arcsine of x
inline float Asin(float x)
{

	return asinf(x);

}

// Calculates the arctan of x
inline float Atan(float x)
{

	return atanf(x);

}

// Copys the sign from one value to another
template <class T>
inline T CopySign(T from, T to)
{

	return from < 0 ? to < 0 ? to : -to : to >= 0 ? to : -to;  

}

// Compares signs, returns true if equal
template <class T>
inline bool SignCompare(T a, T b)
{

	if ((a < 0) && (b < 0))
		return true;

	if ((a >= 0) && (b >= 0))
		return true;

	return false;

}

// Returns the maximum of two values
template <class T>
inline T Max(T a, T b)
{

	return a > b ? a : b;

}

// Returns the minimum of two values
template <class T>
inline T Min(T a, T b)
{

	return a > b ? b : a;

}

// Clamps a value between a minimum and a maximum
template <class T>
inline T Clamp(T value, T min, T max)
{

	return value < min ? min : value > max ? max : value;

}

// Converrs radians to degrees
inline float Degrees(float radians)
{

	return radians / Math::TWO_PI * 360.0f;

}

// Converst degrees to radians
inline float Radians(float degrees)
{

	return degrees / 360.0f * Math::TWO_PI;

}

// Calculates the square root of a value
inline float Sqrt(float x)
{

	return sqrtf(x);

}

// Calculates the square of a value
inline float Sqr(float x)
{

	return x * x;

}

// Calculates the cube of a value
inline float Cube(float x)
{

	return x * x * x;

}