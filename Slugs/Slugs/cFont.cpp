#include "cFont.h"

cFont::cFont() {
	window = NULL;
}

cFont::~cFont() { 
	window = NULL;
}

bool cFont::bLoad(sf::RenderWindow *engine, std::string sFileName) {

	window = engine;

	if (!fnt.LoadFromFile(sFileName)) 
		return false;
	
	text.SetColor(sf::Color(0, 0, 0));
	text.SetFont(fnt);
	text.SetSize(12);
	text.SetPosition(0, 0);
	text.SetStyle(sf::String::Regular);

	return true;
}

void cFont::setColor(sf::Color color) {
	text.SetColor(color);
}

void cFont::setSize(float iSize) {
	text.SetSize(iSize);
}

void cFont::Draw(float iX, float iY, std::string sText) {
	text.SetText(sText);
	text.SetPosition(iX, iY);

  window->Draw(text);	
}