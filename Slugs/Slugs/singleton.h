#pragma once

/*
	class Singleton<T>
	Simple singleton template
*/

template <class T>
class Singleton
{

private:

	static T* instance;

protected:

	Singleton();
	~Singleton();

public:

	static T* Get();
	static void Destroy();

};

#include "singleton.inl"