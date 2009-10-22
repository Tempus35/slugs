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

Resource* ResourceManager::GetResource(char* name)
{

	return resources[name];

}

void ResourceManager::AddResource(char* name, Resource* resource)
{

	resources.insert(pair<char*, Resource*>(name, resource));

}

void ResourceManager::FreeResource(char* name)
{

	void* r = resources[name];

	if (r)
		delete r;

	resources.erase(name);

}

void ResourceManager::FreeAllResources()
{

	hash_map<char*, Resource*>::iterator i;

	for (i = resources.begin(); i != resources.end(); ++ i)
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