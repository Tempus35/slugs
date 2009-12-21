#include "xml.h"

cXML::cXML() {
	root = NULL;
	doc = NULL;
	current = NULL;
}

cXML::~cXML() {
	if (doc != NULL) {
		doc->Clear();
		delete doc;
		doc = NULL;
	}
	root = NULL;
	current = NULL;
}

bool cXML::bLoad(char *sFileName){
	doc = new TiXmlDocument();
	if (!doc->LoadFile(sFileName)) return false;

	//set the root element
	root = doc->FirstChildElement();
	if (root == NULL) return false;

	//set current element to root element
	current = root;

	return true;
}

TiXmlElement *cXML::get_root() {
	return root;
}

TiXmlAttribute *cXML::get_attribute(TiXmlElement *ele) {
	return ele->FirstAttribute();
}

TiXmlAttribute *cXML::get_attribute(TiXmlElement *ele, char *sName) {
	TiXmlAttribute *attrib;
	attrib = ele->FirstAttribute();
	do {
		if (strcmp(attrib->Name(), sName) == 0) 
			return attrib;
		attrib = attrib->Next();
	}
	while (attrib != NULL);
	return NULL;
}

TiXmlAttribute *cXML::get_attribute() {
	return current->FirstAttribute();
}

TiXmlAttribute *cXML::get_attribute(char *sName) {
	TiXmlAttribute *attrib;
	attrib = current->FirstAttribute();
	do {
		if (strcmp(attrib->Name(), sName) == 0) 
			return attrib;
		attrib = attrib->Next();
	}
	while (attrib != NULL);
	return NULL;
}

TiXmlElement *cXML::get_current() {
	return current;
}

void cXML::set_element(char *sName) {
	current = get_element(sName);
}

void cXML::set_element(TiXmlElement *ele) {
	current = ele;
}

TiXmlElement *cXML::get_element(char *sName){
	TiXmlElement *ele;
	ele = root->FirstChildElement();
	do {
		if (strcmp(ele->Value(), sName) == 0) 
			return ele;
		ele = ele->NextSiblingElement();
	}
	while (ele != NULL);	
	return NULL;
}
