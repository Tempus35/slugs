#pragma once

#include <math.h>

#define PI				3.14159f
#define PI_OVER_2		1.57080f
#define PI_OVER_4		0.78540f
#define PI_OVER_180		0.01745f
#define TWO_PI			6.28319f
#define ONE_OVER_PI		0.31831f
#define SMALL_AMOUNT	1e-6f

template <class T>
inline T Abs(T x)
{

	return x < 0 ? -x : x;
	
}

inline int RoundToInt(float f)
{

	return (int)(f + 0.5f);

}

inline int RoundDownToInt(float x)
{

	return (int)floorf(x);

}


inline float Floor(float x)
{

	return floorf(x);

}

inline float Ceil(float x)
{

	return ceilf(x);

}

inline float Round(float f)
{

	return floorf(f + 0.5f);

}

inline float Sin(float x)
{

	return sinf(x);
	
}

inline float Cos(float x)
{
	
	return cosf(x);
	
}

template <class T>
inline T CopySign(T from, T to)
{

	return from < 0 ? to < 0 ? to : -to : to >= 0 ? to : -to;  

}

template <class T>
inline T Max(T a, T b)
{

	return a > b ? a : b;

}

template <class T>
inline T Min(T a, T b)
{

	return a > b ? b : a;

}

template <class T>
inline T Clamp(T value, T min, T max)
{

	return value < min ? min : value > max ? max : value;

}


inline float Degrees(float radians)
{

	return radians / TWO_PI * 360.0f;

}

inline float Radians(float degrees)
{

	return degrees / 360.0f * TWO_PI;

}

inline float Sqrt(float x)
{

	return sqrtf(x);

}

inline float Sqr(float x)
{

	return x * x;

}

inline float Cube(float x)
{

	return x * x * x;

}