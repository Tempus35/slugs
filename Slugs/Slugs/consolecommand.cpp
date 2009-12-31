#include "consolecommand.h"

/*
	class ConsoleCommand
*/

ConsoleCommand::ConsoleCommand(const std::string& desc)
{

	description = desc;

}

const std::string& ConsoleCommand::GetDescription() const
{

	return description;

}

bool ConsoleCommand::Convert(const std::string& text, bool& result) const
{

	if (text == "true")
	{

		result = true;
		return true;

	}
	else if (text == "false")
	{

		result = false;
		return true;

	}

	return false;

}

bool ConsoleCommand::Convert(const std::string& text, int& result) const
{

	result = atoi(text.c_str());
	return true;

}

bool ConsoleCommand::Convert(const std::string& text, float& result) const
{

	result = (float)atof(text.c_str());
	return true;

}