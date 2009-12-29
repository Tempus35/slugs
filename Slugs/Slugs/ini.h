#pragma once

#include <string>
#include <windows.h>

/*
	class IniReader
	Reads strings from a .ini file
*/

class IniReader
{

private:

	std::string		filePath;					// Path to the ini file
	char			buffer[256];				// Internal read buffer

public:

	// Constructor
	IniReader(const std::string& path);

	// Reads values from an ini file
	std::string Read(const std::string& section, const std::string& key, const std::string& defaultValue = "");
	int ReadInt(const std::string& section, const std::string& key, int defaultValue = 0);
	bool ReadBool(const std::string& section, const std::string& key, bool defaultValue = false);

};

/*
	class IniWriter
	Writes strings to an ini file
*/

class IniWriter
{

private:

	std::string		filePath;					// Path to the ini file
	char			buffer[256];				// Internal write buffer

public:

	// Constructor
	IniWriter(const std::string& path);

	// Writes a key/value pair to an ini section
	void Write(const std::string& section, const std::string& key, const std::string& value);
	void WriteInt(const std::string& section, const std::string& key, int value);
	void WriteBool(const std::string& section, const std::string& key, bool value);

};