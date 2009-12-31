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

	// Converts a string argument into a bool
	bool Convert(const std::string& text, bool& result) const;
	
	// Converts a string argument into an int
	bool Convert(const std::string& text, int& result) const;

	// Converts a string argument into a float
	bool Convert(const std::string& text, float& result) const;

public:

	// Constructor
	ConsoleCommand(const std::string& desc);

	// Gets the description for the command
	const std::string& GetDescription() const;

	// Overriden by subclasses to execute their commands
	virtual bool Call(const std::vector<std::string>& args) = 0;

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

/*
	class ConsoleCommand_1
	A console command taking one argument
	T is the type of object on which the command operates
*/

template <class T, class A>
class ConsoleCommand_1 : public ConsoleCommand
{

protected:

	void (T::*function)(A);						// Pointer to function to execute	
	T* object;									// Object on which the function is called

public:

	ConsoleCommand_1(const std::string& desc, T* target, void(T::*callback)(A));

	virtual bool Call(const std::vector<std::string>& args);

};

/*
	class ConsoleCommand_2
	A console command taking two arguments
	T is the type of object on which the command operates
*/

template <class T, class A, class B>
class ConsoleCommand_2 : public ConsoleCommand
{

protected:

	void (T::*function)(A, B);			// Pointer to function to execute	
	T* object;									// Object on which the function is called

public:

	ConsoleCommand_2(const std::string& desc, T* target, void(T::*callback)(A, B));

	virtual bool Call(const std::vector<std::string>& args);

};

#include "consolecommand.inl"