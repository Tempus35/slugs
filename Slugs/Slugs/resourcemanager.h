#pragma once

/*
	class Resources
	Resource Manager
*/

#include <hash_map>

using namespace std;
using namespace stdext;

#include "singleton.h"
#include "defs.h"
#include "resources.h"

class ResourceManager : public Singleton<ResourceManager>
{

friend class Singleton<ResourceManager>;

private:

	hash_map<char*, Resource*> resources;

	//
	// Initialization
	//
	
	ResourceManager();
	~ResourceManager();

public:

	//
	// Management
	//

	Resource* GetResource(char* name);
	void AddResource(char* name, Resource* resource);
	void FreeResource(char* name);
	void FreeAllResources();

	//
	// Accessors
	//

	int NumResources();

};