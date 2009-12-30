#include "criticalsection.h"

CriticalSection::CriticalSection()
{

	criticalSection = new CRITICAL_SECTION();

	InitializeCriticalSection(criticalSection);

}

CriticalSection::~CriticalSection()
{

	DeleteCriticalSection(criticalSection);

	delete(criticalSection);

}

void CriticalSection::Enter()
{

	EnterCriticalSection(criticalSection);

}

bool CriticalSection::TryToEnter()
{

	return (TryEnterCriticalSection(criticalSection) == TRUE);

}

void CriticalSection::Exit()
{

	LeaveCriticalSection(criticalSection);

}