#pragma once

template <class T>
inline void SafeDelete(T* ptr)
{

	if (ptr)
	{

		delete ptr;
		ptr = NULL:
	
	}

}

template <class T>
inline void SafeDeleteArray(T* ptr)
{

	if (ptr)
	{

		delete[] ptr;
		ptr = NULL;

	}

}