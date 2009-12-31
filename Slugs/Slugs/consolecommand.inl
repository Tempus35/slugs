/*
	class ConsoleCommand_0
*/

template <class T>
ConsoleCommand_0<T>::ConsoleCommand_0(const std::string& desc, T* target, void(T::*callback)(void)) : ConsoleCommand(desc)
{

	object = target;
	function = callback;

}

template <class T>
bool ConsoleCommand_0<T>::Call(const std::vector<std::string>& args)
{

	if (args.size() != 0)
		return false;

	(object->*function)();

	return true;

}

/*
	class ConsoleCommand_1
*/

template <class T, class A>
ConsoleCommand_1<T, A>::ConsoleCommand_1(const std::string& desc, T* target, void(T::*callback)(A)) : ConsoleCommand(desc)
{

	object = target;
	function = callback;

}

template <class T, class A>
bool ConsoleCommand_1<T, A>::Call(const std::vector<std::string>& args)
{

	if (args.size() != 1)
		return false;

	A arg0;

	if (!Convert(args[0], arg0))
		return false;
	
	(object->*function)(arg0);

	return true;

}

/*
	class ConsoleCommand_2
*/

template <class T, class A, class B>
ConsoleCommand_2<T, A, B>::ConsoleCommand_2(const std::string& desc, T* target, void(T::*callback)(A, B)) : ConsoleCommand(desc)
{

	object = target;
	function = callback;

}

template <class T, class A, class B>
bool ConsoleCommand_2<T, A, B>::Call(const std::vector<std::string>& args)
{

	if (args.size() != 2)
		return false;

	A arg0;
	B arg1;
	
	if (!Convert(args[0], arg0))
		return false;

	if (!Convert(args[1], arg1))
		return false;
	
	(object->*function)(arg0, arg1);

	return true;

}