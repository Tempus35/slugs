#pragma once

#include <sfml/graphics.hpp>

#include "global.h"

struct Color
{

	// Predefined colors
	static const Color red, green, blue, yellow, cyan, purple, black, white;

	// Red, green, blue, alpha values (0 - 255)
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

	// Convert to an sf::Color structure
	sf::Color ToSF() const
	{

		return sf::Color(r, g, b, a);

	}

};