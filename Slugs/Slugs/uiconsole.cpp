#include "uiconsole.h"

/*
	class UIConsole
*/

UIConsole::UIConsole(int maxOutput, int historySize)
{

	maxLines = maxOutput;
	maxHistory = historySize;

	text = "";
	historyIt = history.begin();

	open = false;

}

void UIConsole::Register(const std::string& name, ConsoleCommand* command)
{

	ASSERT(text != "");
	ASSERT(command != NULL);
	ASSERT(Find(name) == NULL);

	// Register the new command
	commands.push_back(std::pair<std::string, ConsoleCommand*>(name, command));

}

ConsoleCommand* UIConsole::Find(const std::string& name)
{

	for (unsigned int i = 0; i < commands.size(); ++ i)
	{

		if (commands[i].first == name)
			return commands[i].second;

	}

	return NULL;

}

void UIConsole::Execute()
{

	if (text != "")
	{

		Execute(text);

		AddToHistory(text);
		text = "";

	}

}

void UIConsole::Execute(const std::string& command, bool silent, bool addToHistory)
{

	if (command != "")
	{

		std::string name;
		std::vector<std::string> args;

		ConsoleCommand* c = Find(name);

		if (c != NULL)
		{

			bool result = c->Call(args);

			if (!result)
				Print("Invalid args for command '%s'.", name);

		}
		else
			Print("Command '%s' is not a valid command.", name);

		if (addToHistory)
			AddToHistory(command);

		historyIt = history.begin();

	}

}

void UIConsole::Clear()
{

	text = "";
	output.clear();

}

void UIConsole::ClearHistory()
{

	history.clear();

}

void UIConsole::Help()
{

#ifdef _DEBUG

	for (unsigned int i = 0; i < commands.size(); ++ i)
		Print("[%s] %s", commands[i].first, commands[i].second->GetDescription());

#endif

}

void UIConsole::Print(const std::string& format, ...)
{

	if (output.size() == maxLines)
		output.pop_back();

	va_list args;
	va_start(args, format);

	vsprintf_s(buffer, MAX_COMMAND_LENGTH, format.c_str(), args);

	output.push_front(buffer);

	va_end(args);

}

void UIConsole::AddToHistory(const std::string& command)
{

	ASSERT(command != "");

	if (history.size() == maxHistory)
		history.pop_back();

	history.push_front(command);

}