#pragma once

#include <vector>
#include <list>
#include <string>
#include <stdarg.h>

#include "uiwidget.h"
#include "consolecommand.h"

/*
	class UIConsole
	A console which rolls down from the top of the screen
	Used to enter debugging commands
*/

class UIConsole : public UIWidget
{

private:

	static const int											MAX_COMMAND_LENGTH = 1024;		// Maximum length of a command string including args

protected:

	std::vector<std::pair<std::string, ConsoleCommand*>>		commands;						// Registered commands
	int															maxHistory;						// Maximum number of history entries
	std::list<std::string>										history;						// History of command strings entered
	std::list<std::string>::iterator							historyIt;						// Current history element	
	std::string													text;							// Current command line
	int															maxLines;						// Maximum number of output lines
	std::list<std::string>										output;							// Output lines
	char														buffer[MAX_COMMAND_LENGTH];		// Output format buffer
	bool														open;							// Is the console currently open?

public:

	// Constructor
	UIConsole(int maxOutput = 1024, int historySize = 1024);

	// Registers a console command
	void Register(const std::string& name, ConsoleCommand* command);

	// Searches for an existing command
	ConsoleCommand* Find(const std::string& name);

	// Executes the current command line
	void Execute();

	// Executes the given command
	void Execute(const std::string& command, bool silent = false, bool addToHistory = true);

	// Clears the current console line and the output store
	void Clear();

	// Clears the console history
	void ClearHistory();

	// Prints all console commands to the console
	void Help();

	// Prints a line to the console
	void Print(const std::string& format, ...);

	// Adds a command to the history list
	void AddToHistory(const std::string& command);

};