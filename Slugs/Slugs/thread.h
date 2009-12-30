#pragma once

#include <windows.h>
#include <process.h>

#include "global.h"

/*
	Forward declarations
*/

class Thread;

/*
	struct ThreadData
*/

struct ThreadData
{

	Thread*		object;
	void*		argument;

};

/*
	class Thread
	A background worker thread
*/

class Thread
{

protected:

	static const HANDLE					mainThread;

protected:

	HANDLE								handle;			// Thread handle
	unsigned int						id;				// Thread id

	bool								running;		// Is the thread currently running?

	HANDLE								stop;			// Event used to stop the thread

	ThreadData*							internalData;

protected:

	static unsigned int WINAPI Run(void* data);

protected:

	// Constructor
	Thread();

	// The DoWork method should check this periodically and if true, should stop immediately
	bool ShouldStop() const;


public:

	// Returns true if calling thread is the main thread
	static bool IsMainThread();

public:

	// Destructor
	~Thread();

	// Starts the thread passing in an argument
	bool Start(void* argument = NULL);

	// Stops the thread (if kill is true, the thread is forced to exit)
	void Stop(bool kill = false);

	// Must be overriden in derived classes to perform the work required
	virtual unsigned int DoWork(void* object) = 0;

	// Returns true if the thread is currently running
	bool IsRunning() const;

	// Returns the thread id
	unsigned int GetId() const;

};