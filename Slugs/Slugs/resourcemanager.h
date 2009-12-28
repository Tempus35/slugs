#pragma once

#include <hash_map>
#include <string>

#include "singleton.h"
#include "global.h"
#include "resource.h"
#include "imageresource.h"
#include "soundresource.h"
#include "fontresource.h"
#include "textresource.h"

/*
	class ResourceManager
	Manager for all resources
*/

class ResourceManager : public Singleton<ResourceManager>
{

friend class Singleton<ResourceManager>;

private:

	stdext::hash_map<std::string, Resource*> resources;				// Map of resource names to resource pointers

	// Constructor
	ResourceManager();

	// Destructor
	~ResourceManager();

public:

	// Gets a resource by name
	Resource* GetResource(const std::string& name);
	ImageResource* GetImage(const std::string& name);
	SoundResource* GetSound(const std::string& name);
	FontResource* GetFont(const std::string& name);
	TextResource* GetText(const std::string& name);

	// Adds a resource to the manager - takes ownership of the resource instance
	void AddResource(const std::string& name, Resource* resource);

	// Frees memory associated with a resource
	void FreeResource(const std::string& name);

	// Frees all currently loaded resources
	void FreeAllResources();

	// Gets the number of resources currently loaded
	int NumResources() const;

};