#pragma once

#include <windows.h>

// Safely deletes
template <class T>
inline void SafeDelete(T* ptr)
{

	if (ptr)
	{

		delete ptr;
		ptr = NULL;
	
	}

}

// Safely deletes an array
template <class T>
inline void SafeDeleteArray(T* ptr)
{

	if (ptr)
	{

		delete[] ptr;
		ptr = NULL;

	}

}

// Closes an open HANDLE
inline void SafeCloseHandle(HANDLE h)
{

	if (h)
	{

		CloseHandle(h);
		h = NULL;

	}

}