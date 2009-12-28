#include "textresource.h"

TextResource::TextResource(const std::string& path)
{

	std::ifstream fs(path.c_str(), std::ios_base::in);
	char buffer[1024];
	Line line;
	
	if (fs.is_open())
	{

		while (fs.getline(buffer, 1024))
		{

			line.text = buffer;
			lines.push_back(line);

		}

		fs.close();

	}

	numUsed = 0;

}

TextResource::TextResource(const std::vector<std::string>& strings)
{

	Line line;
	for (unsigned int i = 0; i < strings.size(); ++ i)
	{

		line.used = false;
		line.text = strings[i];

		lines.push_back(line);

	}

	numUsed = 0;

}

void TextResource::GetLine(std::string& line, unsigned int index, bool markAsUsed)
{

	if ((index >= 0) && (index < lines.size()))
	{

		if (markAsUsed)
		{

			lines[index].used = true;
			numUsed ++;

		}

		line = lines[index].text;

	}

}

bool TextResource::IsLineUsed(unsigned int index)
{

	if ((index >= 0) && (index < lines.size()))
		return lines[index].used;

	return false;

}

void TextResource::GetRandomLine(std::string& line, bool markAsUsed)
{

	if (numUsed == lines.size())
		return;

	int numAttempts = 5;
	int pick;
	while (numAttempts > 0)
	{

		pick = Random::RandomInt(0, lines.size() - 1);

		if (!lines[pick].used)
		{
		
			GetLine(line, pick, markAsUsed);
			return;

		}

		numAttempts --;

	}

	GetFirstUnusedLine(line, markAsUsed);

}

void TextResource::GetFirstUnusedLine(std::string& line, bool markAsUsed)
{

	if (numUsed == lines.size())
		return;

	for (unsigned int i = 0; i < lines.size(); ++ i)
	{

		if (lines[i].used == false)
		{

			if (markAsUsed)
				lines[i].used = true;

			line = lines[i].text;

		}

	}

}

void TextResource::ClearFlags()
{

	for (unsigned int i = 0; i < lines.size(); ++ i)
		lines[i].used = false;

	numUsed = 0;

}