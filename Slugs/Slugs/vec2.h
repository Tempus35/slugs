#pragma once

#include <math.h>
#include <sfml/graphics.hpp>

/*
	struct Vec2
	Two dimensional vector
*/

template <class T>
struct Vec2
{

	T x, y;

	Vec2() { x = y = 0; };
	Vec2(T X, T Y) { x = X; y = Y; };
	Vec2(sf::Vector2f v) { x = v.x; y = v.y; };
	Vec2(sf::Vector2i v) { x = v.x; y = v.y; };

	Vec2& operator = (const Vec2& v)
	{

		if (this != &v)
		{

			x = v.x;
			y = v.y;

		}

		return *this;

	}

	Vec2& operator *= (T f)
	{

		x *= f;
		y *= f;

		return *this;

	}

	Vec2& operator /= (T f)
	{

		x /= f;
		y /= f;

		return *this;

	}

	Vec2& operator += (const Vec2& v)
	{

		x += v.x;
		y += v.y;

		return *this;

	}

	Vec2& operator -= (const Vec2& v)
	{

		x -= v.x;
		y -= v.y;

		return *this;

	}

	Vec2 operator + (const Vec2& v)
	{

		Vec2 result = *this;
		result += v;

		return result;

	}

	Vec2 operator - (const Vec2& v)
	{

		Vec2 result = *this;
		result -= v;

		return result;
	}

	Vec2 operator - ()
	{

		Vec2 v = *this;
		v.x = -v.x;
		v.y = -v.y;

		return v;

	}

	Vec2 operator * (T f)
	{

		Vec2 result = *this;
		result *= f;

		return result;

	}

	Vec2 operator / (T f)
	{

		Vec2 result = *this;
		result /= f;

		return result;

	}

	inline float LengthSquared()
	{

		return x * x + y * y;

	}

	inline float Length()
	{

		return Sqrt(LengthSquared());

	}

	Vec2<float> Normalize()
	{

		float lengthSquared = LengthSquared();

		if (lengthSquared > 0.0f)
		{

			float length = Sqrt(lengthSquared);
			return Vec2<float>(x / length, y / length);

		}

		return Vec2<float>(x, y);

	}

	Vec2 Perpendicular()
	{

		return Vec2(-y, x);

	}

	sf::Vector2<T> ToSF()
	{

		return sf::Vector2<T>(x, -y);

	}

};

//
// Predefined types
//

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;

//
// Methods
//

template <class T>
inline float DotProduct(const Vec2<T>& a, const Vec2<T>& b)
{

	return a.x * b.x + a.y * b.y;

}

//
// Operators
//

template <class T>
inline Vec2<T> operator + (const Vec2<T>& a, const Vec2<T>& b)
{

	return Vec2<T>(a.x + b.x, a.y + b.y);

}

template <class T>
inline Vec2<T> operator - (const Vec2<T>& a, const Vec2<T>& b)
{

	return Vec2<T>(a.x - b.x, a.y - b.y);

}

template <class T>
inline Vec2<T> operator * (const Vec2<T>& v, float f)
{

	return Vec2<T>(v.x * f, v.y * f);

}

template <class T>
inline Vec2<T> operator / (const Vec2<T>& v, float f)
{

	return Vec2<T>(v.x / f, v.y / f);

}