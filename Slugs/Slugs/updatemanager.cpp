//---------------------------------------------------------------
//
// Slugs
// updatemanager.cpp
//
//---------------------------------------------------------------

#include "updatemanager.h"

/*
	class UpdateManager
*/

bool UpdateManager::IsRegistered(Updateable* obj)
{

	std::list<Updateable*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		if (*i == obj)
			return true;

		i++;

	}

	return false;

}

void UpdateManager::Update(float elapsedTime)
{

	//
	// Unregister objects
	//

	if (unregistering.size() > 0)
	{

		for (unsigned int i = 0; i < unregistering.size(); ++ i)
			objects.remove(unregistering[i]);

		unregistering.clear();

	}

	//
	// Dispatch update messages
	//

	std::list<Updateable*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		(*i)->Update(elapsedTime);

		i++;

	}

}

void UpdateManager::RegisterForUpdates(Updateable* obj)
{

	if (!IsRegistered(obj))
		objects.push_back(obj);

}

void UpdateManager::UnregisterForUpdates(Updateable* obj)
{

	//
	// The object is not immediately unregistered, as doing so would break and current update
	// It is removed from the list at the start on the next update
	//

	unregistering.push_back(obj);

}

void UpdateManager::UnregisterAll()
{

	objects.clear();

}
