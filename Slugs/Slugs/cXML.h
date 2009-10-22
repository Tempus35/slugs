#pragma once

#include "globals.h"

class cXML 
{
public:
	cXML();
	~cXML();
	bool bLoad(char *sFileName);

	TiXmlElement *get_root();

	void set_element(char *sName);
	void set_element(TiXmlElement *ele);

	TiXmlAttribute *get_attribute(TiXmlElement *ele);
	TiXmlAttribute *get_attribute(TiXmlElement *ele, char *sName);

	TiXmlAttribute *get_attribute();
	TiXmlAttribute *get_attribute(char *sName);

	TiXmlElement *get_element(char *sName);
	TiXmlElement *get_current();

private:
	TiXmlDocument *doc;
	TiXmlElement *root;
	TiXmlElement *current;
};