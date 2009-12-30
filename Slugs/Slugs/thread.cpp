#include "thread.h"

const HANDLE Thread::mainThread = GetCurrentThread();

Thread::Thread()
{

	running = false;
	handle = NULL;
	id = 0;
	stop = CreateEvent(NULL, false, false, NULL);
	internalData = NULL;

}

Thread::~Thread()
{

	if (running)
		SetEvent(stop);

	SafeDelete(internalData);

}

unsigned int WINAPI Thread::Run(void* data)
{

	ThreadData* threadData = (ThreadData*)data;

	unsigned int result = threadData->object->DoWork(threadData->argument);

	threadData->object->running = false;

	return result;

}

bool Thread::ShouldStop() const
{

	return (WaitForSingleObject(stop, 0) == WAIT_OBJECT_0);

}

bool Thread::IsMainThread()
{

	return GetCurrentThread() == mainThread;

}

bool Thread::Start(void* argument)
{

	if (running)
		return false;

	SafeDelete(internalData);
	internalData = new ThreadData();
	internalData->object = this;
	internalData->argument = argument;

	handle = (HANDLE)_beginthreadex(NULL, 0, &Thread::Run, internalData, 0, &id);

	if (handle)
	{

		running = true;
		return true;

	}

	return false;

}

void Thread::Stop(bool kill)
{

	if (running)
	{

		if (kill)
			TerminateThread(handle, 0);
		else
			SetEvent(stop);
		
	}
		
}

bool Thread::IsRunning() const
{

	return running;

}

unsigned int Thread::GetId() const
{

	return id;

}