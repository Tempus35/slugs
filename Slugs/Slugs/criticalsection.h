#pragma once

#include <windows.h>

/*
	class CriticalSection
	Used to prevent multiple threads from entering the same block of code
*/

class CriticalSection
{

private:

	CRITICAL_SECTION*	criticalSection;				// Pointer to critical section object

public:

	// Constructor
	CriticalSection();

	// Destructor
	~CriticalSection();

	// Enters the critical section if possible, otherwise blocks execution
	void Enter();

	// Enters the critical section if possible
	bool TryToEnter();

	// Leaves the critical section
	void Exit();

};