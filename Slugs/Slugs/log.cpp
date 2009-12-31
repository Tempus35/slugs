#include "log.h"

Log::Log()
{

	mode = LogMode_Text;

}

Log::~Log()
{

	Close();

}

void Log::Open(const std::string& path, LogMode _mode)
{

	#ifdef LOGGING_ENABLED

		mode = _mode;
		writer.Open(path);

	#endif

}

void Log::Close()
{

	writer.Close();

}

void Log::Write(LogType type, const char* format, ...)
{

	va_list args;
	va_start(args, format);

	Write(type, format, args);

	va_end(args);

}

void Log::Write(LogType type, const char* format, va_list args)
{

	#ifdef LOGGING_ENABLED

		char* lineFormat;

		switch (mode)
		{

		case LogMode_HTML:

			if (type == LogType_Error)
				lineFormat = "<font color=#FF0000><b>%s</b></font><br>";
			else if (type == LogType_Warning)
				lineFormat = "<font color=#0000FF>%s</font><br>";
			else
				lineFormat = "%s<br>";

			break;

		case LogMode_XML:

			lineFormat = "%s";

			break;

		default:

			if (type == LogType_Error)
				lineFormat = "ERROR: %s";
			else if (type == LogType_Warning)
				lineFormat = "WARNING: %s";
			else
				lineFormat = "%s";

			break;

		}

		vsprintf_s(buffer, MAX_LINE_LENGTH, format, args);
		writer.WriteLine(lineFormat, buffer);

	#endif

}