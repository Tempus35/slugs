#include "converters.h"

std::string BoolToString(bool value)
{

	if (value == true)
		return "true";
	else
		return "false";

}

std::vector<std::string> StringSplit(const std::string& string, char delimiter)
{

	std::stringstream stream(string);
	std::vector<std::string> elements;
	std::string e;

	while (std::getline(stream, e, delimiter))
		elements.push_back(e);

	return elements;

}

std::vector<std::string> SplitCommand(std::string& command, const std::string& string, char delimiter)
{

	std::stringstream stream(string);
	std::vector<std::string> elements;
	std::string e;

	bool first = true;

	while (std::getline(stream, e, delimiter))
	{

		if (first)
		{

			command = e;
			first = false;

		}
		else
			elements.push_back(e);

	}

	return elements;

}

Vec2i ConvertVector(const Vec2f& v)
{

	return Vec2i(RoundDownToInt(v.x), RoundDownToInt(v.y));

}

Vec2f ConvertVector(const Vec2i& v)
{

	return Vec2f((float)v.x, (float)v.y);

}

bool SFMLKeyToChar(sf::Key::Code key, bool shift, char& c)
{

	if ((key >= 97) && (key <= 122))
	{

		// Letters
		if (shift)
			c = (char)(key - 32);
		else
			c = (char)key;

		return true;

	}
	else if (key >= 48 && key <= 57)
	{

		if (shift)
		{

			switch (key)
			{

			case sf::Key::Num1:
				c = '!';

			case sf::Key::Num2:
				c = '@';

			case sf::Key::Num3:
				c = '#';

			case sf::Key::Num4:
				c = '$';

			case sf::Key::Num5:
				c = '%';

			case sf::Key::Num6:
				c = '^';

			case sf::Key::Num7:
				c = '&';

			case sf::Key::Num8:
				c = '*';

			case sf::Key::Num9:
				c = '(';

			case sf::Key::Num0:
				c = ')';

			}

		}
		else
			c = (char)key;

		return true;

	}
	else if (key == sf::Key::Space)
	{

		c = ' ';
		return true;

	}
	else if (key == sf::Key::Tilde)
	{

		c = '~';
		return true;

	}
	else if (key == sf::Key::Period)
	{

		c = '.';
		return true;

	}

	return false;

}