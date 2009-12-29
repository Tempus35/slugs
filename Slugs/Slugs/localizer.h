//---------------------------------------------------------------
//
// Slugs
// localizer.h
//
//---------------------------------------------------------------

#pragma once

#include <hash_map>
#include <string>

#include "singleton.h"

/*
	Enumeration of available languages
*/

enum Language
{

	Language_enUS,

};

/*
	class Localizer
	Localizes strings using language files
*/

class Localizer : public Singleton<Localizer>
{

friend class Singleton<Localizer>;

private:

	stdext::hash_map<std::string, std::string>		map;				// Map of string names to localized string text
	Language										currentLanguage;	// Identifier of the current language

public:

	// Initializes the localizer with filenames in language, returns true if all files were available
	// Each filepath is the path to the file without the extension
	bool Initialize(const std::vector<std::string>& filePaths, Language language);

	// Gets a localized string by name
	const std::string& Localize(const std::string& stringName);

	// Gets the current language identifier
	Language GetLanguage() const;

	// Gets the current language as a string
	std::string GetLanguageString() const;

	// Converts a language identifier to a string
	std::string LanguageToString(Language language) const;

	// Converts a string to a language identifier
	Language StringToLanguage(const std::string& string) const;

};