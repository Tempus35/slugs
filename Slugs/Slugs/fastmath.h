#pragma once

#include <math.h>

#define PI				3.14159f
#define PI_OVER_2		1.57080f
#define PI_OVER_4		0.78540f
#define PI_OVER_180		0.01745f
#define TWO_PI			6.28319f
#define ONE_OVER_PI		0.31831f
#define SMALL_AMOUNT	1e-6f

static inline float clamp(float f, float min, float max)
{

	return f < min ? min : f > max ? max : f;

}

static inline int clamp(int i, int min, int max)
{

	return i < min ? min : i > max ? max : i;

}

static inline float fastfabs(float x)
{

	return x < 0 ? -x : x;
	
}

static inline int fastround(float f)
{

	return (int)(f + 0.5f);

}

static inline float fastroundf(float f)
{

	return floorf(f + 0.5f);

}

static inline float fastsinf(float x)
{

	const float P = 0.225f;
	
	x = x * ONE_OVER_PI;
	int k = fastround(x);
	x = x - k;
    
	float y = (4 - 4 * fastfabs(x)) * x;
    
	y = P * (y * fastfabs(y) - y) + y;
    
	return (k & 1) ? -y : y;
	
}

static inline float fastcosf(float c)
{
	
	float x = c + PI_OVER_2;
	const float P = 0.225f;
	
	x = x * ONE_OVER_PI;
	int k = fastround(x);
	x = x - k;
    
	float y = (4 - 4 * fastfabs(x)) * x;
    
	y = P * (y * fastfabs(y) - y) + y;
    
	return (k & 1) ? -y : y;
	
}

static inline float copysign(float from, float to)
{

	return from < 0 ? to < 0 ? to : -to : to >= 0 ? to : -to;  

}