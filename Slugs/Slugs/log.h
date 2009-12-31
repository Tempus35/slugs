#pragma once

#include <stdarg.h>
#include <string>

#include "singleton.h"
#include "textwriter.h"

// Comment this line to disable logging
#define LOGGING_ENABLED

enum LogType
{

	LogType_Message,
	LogType_Warning,
	LogType_Error,

};

enum LogMode
{

	LogMode_Text,
	LogMode_HTML,
	LogMode_XML,

};

class Log : public Singleton<Log>
{

friend class Singleton<Log>;

private:

	static const int	MAX_LINE_LENGTH = 1024;

	TextWriter			writer;
	LogMode				mode;
	char				buffer[MAX_LINE_LENGTH];

private:

	Log();
	~Log();

public:

	void Open(const std::string& path = "log", LogMode _mode = LogMode_HTML);
	void Close();

	void Write(LogType type, const char* format, ...);
	void Write(LogType type, const char* format, va_list args);

};