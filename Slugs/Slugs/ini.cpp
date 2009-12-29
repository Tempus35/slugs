#include "ini.h"

/*
	class IniReader
*/

IniReader::IniReader(const std::string& path)
{

	filePath = path;

}

std::string IniReader::Read(const std::string& section, const std::string& key, const std::string& defaultValue)
{

	GetPrivateProfileString(section.c_str(), key.c_str(), defaultValue.c_str(), buffer, 256, filePath.c_str()); 

	return buffer;

}

int IniReader::ReadInt(const std::string& section, const std::string& key, int defaultValue)
{

	GetPrivateProfileString(section.c_str(), key.c_str(), "", buffer, 256, filePath.c_str()); 

	if (_stricmp(buffer, "") == 0)
		return defaultValue;

	return atoi(buffer);

}

bool IniReader::ReadBool(const std::string& section, const std::string& key, bool defaultValue)
{

	GetPrivateProfileString(section.c_str(), key.c_str(), "", buffer, 256, filePath.c_str()); 

	if (_stricmp(buffer, "") == 0)
		return defaultValue;
	else if (_stricmp(buffer, "false") == 0)
		return false;
	else
		return true;

}

/*
	class IniWriter
*/

IniWriter::IniWriter(const std::string& path)
{

	filePath = path;

}

void IniWriter::Write(const std::string& section, const std::string& key, const std::string& value)
{

	WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), filePath.c_str());

}

void IniWriter::WriteInt(const std::string& section, const std::string& key, int value)
{

	sprintf_s(buffer, 256, "%i", value);
	WritePrivateProfileString(section.c_str(), key.c_str(), buffer, filePath.c_str());
	
}

void IniWriter::WriteBool(const std::string& section, const std::string& key, bool value)
{

	if (value == false)
		sprintf_s(buffer, 256, "false");
	else
		sprintf_s(buffer, 256, "true");

	WritePrivateProfileString(section.c_str(), key.c_str(), buffer, filePath.c_str());

}