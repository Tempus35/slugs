#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "debug.h"
#include "resource.h"
#include "random.h"

/*
	class TextResource
	Resource container for a text data
*/

class TextResource : public Resource
{

private:

	struct Line
	{

		bool used;
		std::string text;

		Line() { used = false; }

	};

private:

	std::vector<Line>		lines;	
	int						numUsed;
	std::string				empty;

public:

	// Constructors
	TextResource(const std::string& path);
	TextResource(const std::vector<std::string>& strings);

	// Tests a line to see if it is in use
	bool IsLineUsed(unsigned int index);

	// Gets an unused line
	const std::string& GetLine(unsigned int index, bool markAsUsed = false);

	// Gets a random unused line
	const std::string& GetRandomLine(bool markAsUsed = false);

	// Gets the first unused line
	const std::string& GetFirstUnusedLine(bool markAsUsed = false);

	// Gets the number of lines
	unsigned int GetNumLines() const;

	// Clears all used flags
	void ClearFlags();

};