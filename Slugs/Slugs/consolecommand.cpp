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

bool ConsoleCommand::ConvertBool(const std::string& text, bool& result) const
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

int ConsoleCommand::ConvertInt(const std::string& text, int& result) const
{

	result = atoi(text.c_str());
	return true;

}

float ConsoleCommand::ConvertFloat(const std::string& text, float& result) const
{

	result = (float)atof(text.c_str());
	return true;

}

/*
	class ConsoleCommand_0
*/

template <class T>
ConsoleCommand_0<T>::ConsoleCommand_0(const std::string& desc, T* target, void(T::*callback)(void)) : ConsoleCommand(desc)
{

	object = target;
	function = callback;

}

template <class T>
bool ConsoleCommand_0<T>::Call(const std::vector<std::string>& args)
{

	if (args.size() != 0)
		return false;

	(object->*function)();

	return true;

}