#pragma once

#include <math.h>
#include <sfml/graphics.hpp>

/*
	struct Vector2
	Two dimensional floating point vector
*/

struct Vector2
{

	float x, y;

	Vector2() { x = y = 0.0f; };
	Vector2(float X, float Y) { x = X; y = Y; };
	Vector2(int X, int Y) { x = (float)X; y = (float)Y; };
	Vector2(sf::Vector2f v) { x = v.x; y = v.y; };

	Vector2& operator = (const Vector2& v)
	{

		if (this != &v)
		{

			x = v.x;
			y = v.y;

		}

		return *this;

	}

	Vector2& operator *= (float f)
	{

		x *= f;
		y *= f;

		return *this;

	}

	Vector2& operator /= (float f)
	{

		x /= f;
		y /= f;

		return *this;

	}

	Vector2& operator += (const Vector2& v)
	{

		x += v.x;
		y += v.y;

		return *this;

	}

	Vector2& operator -= (const Vector2& v)
	{

		x -= v.x;
		y -= v.y;

		return *this;

	}

	Vector2 operator + (const Vector2& v)
	{

		Vector2 result = *this;
		result += v;

		return result;

	}

	Vector2 operator - (const Vector2& v)
	{

		Vector2 result = *this;
		result -= v;

		return result;
	}

	Vector2 operator - ()
	{

		Vector2 v = *this;
		v.x = -v.x;
		v.y = -v.y;

		return v;

	}

	Vector2 operator * (float f)
	{

		Vector2 result = *this;
		result *= f;

		return result;

	}

	Vector2 operator / (float f)
	{

		Vector2 result = *this;
		result /= f;

		return result;

	}

	sf::Vector2f ToSF()
	{

		return sf::Vector2f(x, y);

	}

};

inline Vector2 operator + (const Vector2& a, const Vector2& b)
{

	return Vector2(a.x + b.x, a.y + b.y);

}

inline Vector2 operator - (const Vector2& a, const Vector2& b)
{

	return Vector2(a.x - b.x, a.y - b.y);

}

inline Vector2 operator * (const Vector2& v, float f)
{

	return Vector2(v.x * f, v.y * f);

}

inline float VectorDot(const Vector2& a, const Vector2& b)
{

	return a.x * b.x + a.y * b.y;

}

inline float VectorLength(const Vector2& v)
{

	return sqrtf(v.x * v.x + v.y * v.y);

}

inline float VectorLengthSquared(const Vector2& v)
{

	return VectorDot(v, v);

}

inline Vector2* VectorNormalize(Vector2* v)
{

	float lengthSquared = v->x * v->x + v->y * v->y;

	if (lengthSquared > 0.0f)
	{

		lengthSquared = 1.0f / sqrtf(lengthSquared);
		v->x *= lengthSquared;
		v->y *= lengthSquared;

	}

	return v;

}