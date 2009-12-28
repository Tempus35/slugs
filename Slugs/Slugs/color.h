#pragma once

#include "global.h"

struct Color
{

	byte r, g, b, a;

	Color()
	{

		r = g = b = a = 0xFF;

	}

	Color(byte red, byte green, byte blue, byte alpha = 0xFF)
	{

		r = red; g = green; b = blue; a = alpha;

	}

	Color(const Color& rgb, byte alpha)
	{

		r = rgb.r;
		g = rgb.g;
		b = rgb.b;
		a = alpha;

	}

	sf::Color ToSF() const
	{

		return sf::Color(r, g, b, a);

	}

};