#pragma once

#include <vector>

#include "slugobject.h"

using namespace std;

class Team
{

private:

	char* name;							// Team Name
	vector<SlugObject*> slugs;			// Vector containing slugs on team
	int numAlive;						// Number of alive slugs remaining
	int activeIndex;					// Index of currently active slug
	int maxHealth;						// Total initial health of the team
	int currentHealth;					// Current health of the team

public:

	Team();
	~Team();

	char* Name();
	void SetName(char* newName);

	bool Add(SlugObject* slug);
	void Reset();

	SlugObject* Next();
	SlugObject* Previous();
	SlugObject* First();
	SlugObject* Last();

};