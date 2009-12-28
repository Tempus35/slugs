#include "resourcemanager.h"

/*
	class ResourceManager
*/

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

	FreeAllResources();

}

Resource* ResourceManager::GetResource(const std::string& name)
{

	return resources[name];

}

ImageResource* ResourceManager::GetImage(const std::string& name)
{

	return (ImageResource*)resources[name];

}

SoundResource* ResourceManager::GetSound(const std::string& name)
{

	return (SoundResource*)resources[name];

}

FontResource* ResourceManager::GetFont(const std::string& name)
{

	return (FontResource*)resources[name];

}

TextResource* ResourceManager::GetText(const std::string& name)
{

	return (TextResource*)resources[name];

}

void ResourceManager::AddResource(const std::string& name, Resource* resource)
{

	resources.insert(std::pair<std::string, Resource*>(name, resource));

}

void ResourceManager::FreeResource(const std::string& name)
{

	void* r = resources[name];

	if (r)
		delete r;

	resources.erase(name);

}

void ResourceManager::FreeAllResources()
{

	for (stdext::hash_map<std::string, Resource*>::iterator i = resources.begin(); i != resources.end(); ++ i)
		delete i->second;

	resources.clear();

}

int ResourceManager::NumResources() const
{

	return resources.size();

}