#include "resourcemanager.h"

/*
	class ResourceManager
*/

//
// Initialization
//

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

	FreeAllResources();

}

//
// Management
//

Resource* ResourceManager::GetResource(const std::string& name)
{

	return resources[name];

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

//
// Accessors
//

int ResourceManager::NumResources()
{

	return resources.size();

}