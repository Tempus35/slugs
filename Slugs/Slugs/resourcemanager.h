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
#include "criticalsection.h"
#include "thread.h"

/*
	struct QueuedResource
	Contains load info for a resource waiting to be loaded
*/

struct QueuedResource
{

	ResourceType	type;			// Type of the resource
	std::string		name;			// Internal name of the resource
	std::string		path;			// Path to the resource file

};

/*
	class ResourceLoaderThread
	Loads queued resources
*/

class ResourceLoaderThread : public Thread
{

public:

	 unsigned int DoWork(void* object);

};

/*
	class ResourceManager
	Manager for all resources
*/

class ResourceManager : public Singleton<ResourceManager>
{

friend class Singleton<ResourceManager>;
friend class ResourceLoaderThread;

private:

	stdext::hash_map<std::string, Resource*>	resources;				// Map of resource names to resource pointers
	std::queue<QueuedResource>					loadQueue;				// Queue of resources waiting to be loaded
	CriticalSection								criticalSection;		// Critical section used to protect the load queue
	ResourceLoaderThread						workerThread;			// Thread used to load resources in the background

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

	// Queues a resource for loading
	void QueueResource(const std::string& name, ResourceType type, const std::string& path);

	// Loads all queued resources
	void LoadQueuedResources();

	// Processes the load queue (works on a seperate thread)
	void ProcessQueue();

	// Frees memory associated with a resource
	void FreeResource(const std::string& name);

	// Frees all currently loaded resources
	void FreeAllResources();

	// Gets the number of resources currently loaded
	int NumResources() const;

	// Returns true if the resourcemanager is currently processing the load queue
	bool IsLoading() const;

};