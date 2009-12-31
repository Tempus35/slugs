#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>

class TextWriter
{

private:

	static const int		MAX_LINE_LENGTH = 1024;

	char					buffer[MAX_LINE_LENGTH];
	std::ofstream			stream;

public:

	TextWriter();
	TextWriter(const std::string& path);
	~TextWriter();

	void Open(const std::string& path);
	void Close();
	void Write(char* format, ...);
	void WriteLine(char* format, ...);

};