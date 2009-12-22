#pragma once

#include <vector>
#include <string>

#include "debug.h"

/*
	struct ConsoleCommand
	Base class for console commands contained by a UIConsole
*/

class ConsoleCommand
{

protected:

	std::string			description;		// Text description of the command

public:

	// Constructor
	ConsoleCommand(const std::string& desc);

	// Gets the description for the command
	const std::string& GetDescription() const;

	// Overriden by subclasses to execute their commands
	virtual bool Call(const std::vector<std::string>& args) = 0;

	// Converts a string argument into a bool
	bool ConvertBool(const std::string& text, bool& result) const;
	
	// Converts a string argument into an int
	int ConvertInt(const std::string& text, int& result) const;

	// Converts a string argument into a float
	float ConvertFloat(const std::string& text, float& result) const;

};

/*
	class ConsoleCommand_0
	A console command taking no arguments
	T is the type of object on which the command operates
*/

template <class T>
class ConsoleCommand_0 : public ConsoleCommand
{

protected:

	void (T::*function)();						// Pointer to function to execute	
	T* object;									// Object on which the function is called

public:

	ConsoleCommand_0(const std::string& desc, T* target, void(T::*callback)(void));

	virtual bool Call(const std::vector<std::string>& args);

};