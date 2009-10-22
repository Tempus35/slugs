#include "team.h"

Team::Team()
{

	name = "";

}

Team::~Team()
{

	slugs.clear();
	numAlive = 0;
	activeIndex = -1;
	maxHealth = 0;
	currentHealth = 0;

}

char* Team::Name()
{

	return name;

}

void Team::SetName(char* newName)
{

	name = newName;

}

bool Team::Add(SlugObject* slug)
{

	if (activeIndex == -1)
		activeIndex = 0;

	return false;

}

void Team::Reset()
{

	name = "";
	slugs.clear();
	numAlive = 0;
	activeIndex = -1;
	maxHealth = 0;
	currentHealth = 0;

}

SlugObject* Team::Next()
{

	activeIndex ++;

	if (activeIndex == slugs.size())
		activeIndex = 0;

	return slugs[activeIndex];

}

SlugObject* Team::Previous()
{

	activeIndex --;

	if (activeIndex < 0)
		activeIndex = slugs.size() - 1;

	return slugs[activeIndex];;

}

SlugObject* Team::First()
{

	activeIndex = 0;

	return slugs[activeIndex];;

}

SlugObject* Team::Last()
{

	activeIndex = slugs.size() - 1;

	return slugs[activeIndex];;

}