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