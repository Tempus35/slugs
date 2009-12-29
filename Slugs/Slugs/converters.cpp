#include "converters.h"

std::string BoolToString(bool value)
{

	if (value == true)
		return "true";
	else
		return "false";

}

Color ColorFromString(const std::string& string)
{

	int r, g, b;
	sscanf_s(string.c_str(), "%i,%i,%i", &r, &g, &b);

	return Color(r, g, b);

}