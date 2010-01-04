#include "resourcemanager.h"
#include "game.h"

/*
	class ResourceLoaderThread
*/

unsigned int ResourceLoaderThread::DoWork(void* object)
{

	ResourceManager* resourceManager = (ResourceManager*)object;

	while ((resourceManager->loadQueue.size() > 0) && (!ShouldStop()))
	{

		resourceManager->criticalSection.Enter();

		QueuedResource& resource = resourceManager->loadQueue.front();

		switch (resource.type)
		{

		case ResourceType_Image:

			resourceManager->AddResource(resource.name, new ImageResource(resource.path));

			break;

		case ResourceType_Sound:

			resourceManager->AddResource(resource.name, new SoundResource(resource.path));

			break;

		case ResourceType_Font:

			resourceManager->AddResource(resource.name, new FontResource(resource.path));

			break;

		case ResourceType_Text:

			resourceManager->AddResource(resource.name, new TextResource(resource.path));

			break;

		case ResourceType_Buffer:

			resourceManager->AddResource(resource.name, new BufferResource(resource.path));

			break;

		}

		resourceManager->loadQueue.pop();

		resourceManager->criticalSection.Exit();

	}

	return 0;

 }

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

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);
	ASSERT(i != resources.end());

	return i->second;

}

ImageResource* ResourceManager::GetImage(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERT(i != resources.end());
	ASSERT(i->second->GetType() == ResourceType_Image);

	return (ImageResource*)i->second;

}

SoundResource* ResourceManager::GetSound(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERT(i != resources.end());
	ASSERT(i->second->GetType() == ResourceType_Sound);

	return (SoundResource*)i->second;

}

FontResource* ResourceManager::GetFont(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERT(i != resources.end());
	ASSERT(i->second->GetType() == ResourceType_Font);

	return (FontResource*)i->second;

}

TextResource* ResourceManager::GetText(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERT(i != resources.end());
	ASSERT(i->second->GetType() == ResourceType_Text);

	return (TextResource*)i->second;

}

BufferResource* ResourceManager::GetBuffer(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERT(i != resources.end());
	ASSERT(i->second->GetType() == ResourceType_Buffer);

	return (BufferResource*)i->second;

}

void ResourceManager::AddResource(const std::string& name, Resource* resource)
{

	criticalSection.Enter();

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);

	ASSERTMSG(i == resources.end(), "Attempt to load resource with duplicate identifier!");

	// Only add the resource if it doesn't already exist
	if (i == resources.end())
		resources.insert(std::pair<std::string, Resource*>(name, resource));

	criticalSection.Exit();

}

void ResourceManager::QueueResource(const std::string& name, ResourceType type, const std::string& path)
{

	QueuedResource q;
	q.name = name;
	q.type = type;
	q.path = path;

	criticalSection.Enter();
	loadQueue.push(q);
	criticalSection.Exit();

}

void ResourceManager::LoadQueuedResources()
{

	ASSERT(!workerThread.IsRunning());

	workerThread.Start(this);

}

void ResourceManager::FreeResource(const std::string& name)
{

	stdext::hash_map<std::string, Resource*>::iterator i = resources.find(name);
	ASSERT(i != resources.end());

	resources.erase(i);

}

void ResourceManager::FreeAllResources()
{

	// Ensure we arent loading queued resources
	while (workerThread.IsRunning())
	{

		workerThread.Stop();
		Sleep(10);

	}

	// Free all resources
	for (stdext::hash_map<std::string, Resource*>::iterator i = resources.begin(); i != resources.end(); ++ i)
		delete i->second;

	resources.clear();

}

int ResourceManager::NumResources() const
{

	return resources.size();

}

bool ResourceManager::IsLoading() const
{

	return workerThread.IsRunning();

}