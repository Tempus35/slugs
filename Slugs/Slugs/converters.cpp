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