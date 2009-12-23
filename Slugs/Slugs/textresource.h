#pragma once

#include <vector>
#include <string>

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

	};

private:

	std::vector<Line>		lines;	
	int						numUsed;

public:

	// Constructors
	TextResource(const std::string& path);
	TextResource(const std::vector<std::string>& strings);

	// Gets an unused line
	void GetLine(std::string& line, unsigned int index, bool markAsUsed = false);

	// Tests a line to see if it is in use
	bool IsLineUsed(unsigned int index);

	// Gets a random unused line
	void GetRandomLine(std::string& line, bool markAsUsed = false);

	// Gets the first unused line
	void GetFirstUnusedLine(std::string& line, bool markAsUsed = false);

	// Clears all used flags
	void ClearFlags();

};