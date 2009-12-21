#pragma once

#include <crtdbg.h>

//
// ASSERT Macros
//

#ifdef _DEBUG

	#define ASSERT _ASSERT
	#define ASSERTMSG(condition, output) _ASSERT_EXPR(condition, _CRT_WIDE(output))

#else

	#define ASSERT(condition)
	#define ASSERTMSG(condition, output)

#endif