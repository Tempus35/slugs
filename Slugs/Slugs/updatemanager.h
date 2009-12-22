//---------------------------------------------------------------
//
// Slugs
// updatemanager.h
//
//---------------------------------------------------------------

#pragma once

#include <vector>
#include <list>

#include "singleton.h"

/*
	class Updateable
	Derive from this class to be able to register with the update manager
*/

class Updateable
{

public:

	// Constructor
	Updateable() {}

	// Implemented in derived classes to respond to updates
	virtual void Update(float elapsedTime) = 0;

};

/*
	class UpdateManager
	Distributes update events to registered class instances
	This class does not take ownership of the registered objects
	Registered objects should be sure to unregister as soon as they no longer require the update event
*/

class UpdateManager : public Singleton<UpdateManager>
{

friend class Singleton<UpdateManager>;

private:

	std::list<Updateable*>			objects;						// List of registered objects
	std::vector<Updateable*>		unregistering;					// List of objects waiting to be unregistered

public:

	// Determines if an object is registered
	bool IsRegistered(Updateable* obj);

	// Distributes the update event to registered objects
	void Update(float elapsedTime);				

	// Registers an objects for updates
	void RegisterForUpdates(Updateable* obj);	

	// Unregisters an object
	void UnregisterForUpdates(Updateable* obj);			

	// Unregisters all registered objects
	void UnregisterAll();

};