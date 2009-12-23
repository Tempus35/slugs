#pragma once

/*
	class Resources
	Resource Manager
*/

#include <hash_map>
#include <string>

#include "singleton.h"
#include "global.h"
#include "resource.h"
#include "imageresource.h"
#include "soundresource.h"
#include "textresource.h"

class ResourceManager : public Singleton<ResourceManager>
{

friend class Singleton<ResourceManager>;

private:

	stdext::hash_map<std::string, Resource*> resources;

	//
	// Initialization
	//
	
	ResourceManager();
	~ResourceManager();

public:

	//
	// Management
	//

	Resource* GetResource(const std::string& name);
	void AddResource(const std::string& name, Resource* resource);
	void FreeResource(const std::string& name);
	void FreeAllResources();

	//
	// Accessors
	//

	int NumResources();

};