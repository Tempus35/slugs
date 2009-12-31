#include "textwriter.h"

TextWriter::TextWriter()
{

}

TextWriter::TextWriter(const std::string& path)
{

	stream.open(path.c_str());

}

TextWriter::~TextWriter()
{

	if (stream.is_open())
		stream.close();

}

void TextWriter::Open(const std::string& path)
{

	if (stream.is_open())
		stream.close();

	stream.open(path.c_str(), std::ios_base::out|std::ios_base::trunc);

}

void TextWriter::Close()
{

	if (stream.is_open())
		stream.close();

}

void TextWriter::Write(char* format, ...)
{

	if (stream.is_open())
	{

		va_list args;
		va_start(args, format);

		sprintf_s(buffer, MAX_LINE_LENGTH, format, args);
		stream.write(buffer, strlen(buffer));

		stream.flush();

		va_end(args);

	}

}

void TextWriter::WriteLine(char* format, ...)
{

	if (stream.is_open())
	{

		va_list args;
		va_start(args, format);

		vsprintf_s(buffer, MAX_LINE_LENGTH, format, args);
		stream.write(buffer, strlen(buffer));
		stream.write("\n", 1);

		stream.flush();

		va_end(args);

	}

}