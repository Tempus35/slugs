#include "singleton.h"

/*
	class Singleton<T>
	Simple singleton template
*/

template <class T>
T* Singleton<T>::instance = 0;

template <class T>
Singleton<T>::Singleton()
{

}

template <class T>
Singleton<T>::~Singleton()
{

}

template <class T>
T* Singleton<T>::Get()
{

	if (!instance)
		instance = new T();

	return instance;

}

template <class T>
void Singleton<T>::Destroy()
{

	delete instance;
	instance = NULL;

}