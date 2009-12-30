#pragma once

#include <vector>
#include <list>
#include <string>
#include <stdarg.h>

#include "uiwidget.h"
#include "consolecommand.h"

/*
	Enumeration of console states
*/

enum UIConsoleState
{

	UIConsoleState_Opening,
	UIConsoleState_Closing,
	UIConsoleState_Open,
	UIConsoleState_Closed,

};

/*
	class UIConsole
	A console which rolls down from the top of the screen
	Used to enter debugging commands
*/

class UIConsole : public UIWidget
{

private:

	static const int											MAX_COMMAND_LENGTH = 1024;		// Maximum length of a command string including args
	static const float											OPENCLOSE_TIMER;				// Numbers of seconds to open/close the console
	static const float											OPEN_SIZE;						// Size of the console in pixels when fully open

protected:

	std::vector<std::pair<std::string, ConsoleCommand*>>		commands;						// Registered commands
	int															maxHistory;						// Maximum number of history entries
	std::list<std::string>										history;						// History of command strings entered
	std::list<std::string>::iterator							historyIt;						// Current history element	
	std::string													text;							// Current command line
	int															maxLines;						// Maximum number of output lines
	std::list<std::string>										output;							// Output lines
	char														buffer[MAX_COMMAND_LENGTH];		// Output format buffer
	UIConsoleState												state;							// Current state of the console
	float														timer;							// Used when opening/closing the console

	float														border;							// Border size in pixels
	float														bottom;							// Current position of the bottom of the console

	Shape														background;						// Background box

protected:

	// Starts opening or closing the console
	void SetOpen(bool open);

public:

	// Constructor
	UIConsole(const std::string& widgetName, int maxOutput = 1024, int historySize = 1024);

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
	void Print(const std::string& text);

	// Prints a formatted line to the console (this needs to be const char* or vsprintf_s crashes)
	void Print(const char* format, ...);

	// Adds a command to the history list
	void AddToHistory(const std::string& command);

	// Updates the console
	void Update(float elapsedTime);

	// Renders the console
	void Render();

	// Sets the position of the console
	void SetPosition(const Vec2i& position);

	// Toggles the console open or closed
	void Toggle();

	// Handles a key press
	bool KeyUp(sf::Key::Code key, bool shift, bool control, bool alt);

	// Returns true if the consoole is open or opening
	bool IsOpen();

};