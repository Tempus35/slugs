#pragma once

#include <stdlib.h>
#include <time.h>

class Random
{

public:

	static inline unsigned int SetSeed(int seed = 0)
	{

		if (seed)
		{
		
			srand(seed);
			return seed;
		
		}
		else
		{

			unsigned int s = (unsigned int)time(NULL);
			srand(s);

			return s;

		}

	}

	static inline int RandomInt(int min, int max)
	{

		return min + rand() % (max - min + 1);

	}

	static inline float RandomFloat()
	{

		return (float)rand() / (float)RAND_MAX;

	}

	static inline float RandomFloat(float min, float max)
	{

		return min + (max - min) * RandomFloat();

	}

};