//---------------------------------------------------------------
//
// Slugs
// localizer.cpp
//
//---------------------------------------------------------------

#include "localizer.h"
#include "resourcemanager.h"
#include "converters.h"

bool Localizer::Initialize(const std::vector<std::string>& filePaths, Language language)
{

	int numLocalized = 0;

	// Clear any previous localization data
	map.clear();

	// Convert the requested language to its string code
	std::string languageString = LanguageToString(language);

	// Process file paths
	for (unsigned int i = 0; i < filePaths.size(); ++ i)
	{

		// Build the complete file path
		std::string localizedFile = filePaths[i] + "_" + languageString + ".txt";

		// Load the resource
		ResourceManager::Get()->AddResource(localizedFile, new TextResource(localizedFile));
		TextResource* resource = ResourceManager::Get()->GetText(localizedFile);

		if (resource)
		{

			// Process the lines in the text resource into the map
			for (unsigned int j = 0; j < resource->GetNumLines(); ++ j)
			{

				std::string line = resource->GetLine(j);
				std::vector<std::string> parts = StringSplit(line, '=');

				if (parts.size() == 2)
					map.insert(std::pair<std::string, std::string>(parts[0], parts[1]));

			}

			// Unload the resource now we are done with it
			ResourceManager::Get()->FreeResource(localizedFile);

			numLocalized ++;
		}

	}

	// Store the language
	currentLanguage = language;

	return (numLocalized == filePaths.size());

}

const std::string& Localizer::Localize(const std::string& stringName)
{

	return map[stringName];

}

Language Localizer::GetLanguage() const
{

	return currentLanguage;

}

std::string Localizer::GetLanguageString() const
{

	return LanguageToString(currentLanguage);

}

std::string Localizer::LanguageToString(Language language) const
{

	switch (language)
	{

	case Language_enUS:
		return "enUS";

	}

	ASSERTMSG(0, "Unknown language localization code!");
	return "UNKNOWN";
		
}

Language Localizer::StringToLanguage(const std::string& string) const
{

	if (string == "enUS")
		return Language_enUS;

	// Defaults to English-US
	return Language_enUS;

}