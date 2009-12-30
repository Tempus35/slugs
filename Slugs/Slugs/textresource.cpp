#include "textresource.h"

TextResource::TextResource(const std::string& path) : Resource(ResourceType_Text)
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
	empty = "";

}

TextResource::TextResource(const std::vector<std::string>& strings) : Resource(ResourceType_Text)
{

	Line line;
	for (unsigned int i = 0; i < strings.size(); ++ i)
	{

		line.used = false;
		line.text = strings[i];

		lines.push_back(line);

	}

	numUsed = 0;
	empty = "";

}

const std::string& TextResource::GetLine(unsigned int index, bool markAsUsed)
{

	if ((index >= 0) && (index < lines.size()))
	{

		if (markAsUsed)
		{

			lines[index].used = true;
			numUsed ++;

		}

		return lines[index].text;

	}

	return empty;

}

bool TextResource::IsLineUsed(unsigned int index)
{

	if ((index >= 0) && (index < lines.size()))
		return lines[index].used;

	return false;

}

const std::string& TextResource::GetRandomLine(bool markAsUsed)
{

	if (numUsed == lines.size())
		return empty;

	int numAttempts = 5;
	int pick;
	while (numAttempts > 0)
	{

		pick = Random::RandomInt(0, lines.size() - 1);

		if (!lines[pick].used)
			return GetLine(pick, markAsUsed);

		numAttempts --;

	}

	return GetFirstUnusedLine(markAsUsed);

}

const std::string& TextResource::GetFirstUnusedLine(bool markAsUsed)
{

	if (numUsed == lines.size())
		return empty;

	for (unsigned int i = 0; i < lines.size(); ++ i)
	{

		if (lines[i].used == false)
		{

			if (markAsUsed)
				lines[i].used = true;

			return lines[i].text;

		}

	}

	return empty;

}

unsigned int TextResource::GetNumLines() const
{

	return lines.size();

}

void TextResource::ClearFlags()
{

	for (unsigned int i = 0; i < lines.size(); ++ i)
		lines[i].used = false;

	numUsed = 0;

}