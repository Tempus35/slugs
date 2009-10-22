#pragma once

#include "globals.h"

class cFont 
{
public:
	cFont();
	~cFont();
	bool bLoad(sf::RenderWindow *engine, std::string sFileName);
	void setSize(float iSize);
	void setColor(sf::Color color);
	void Draw(float iX, float iY, std::string sText);

private:
	sf::Font fnt;
	sf::String text;
	sf::RenderWindow *window;
};