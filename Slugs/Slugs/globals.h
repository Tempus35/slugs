#pragma once

// Do not delete
#define PB_DEBUG

//---SFML---
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include <assert.h>
#include <string>

//test things
#include <vector>
#include <list>

//---TinyXML---
#include <tinystr.h>
#include <tinyxml.h>

//---System---
#include <stdio.h>
#include <math.h>

//---Const---
#define SCENE_LOGO		1
#define SCENE_INTRO		2
#define SCENE_MENUE		3
#define SCENE_GAME		4
#define SCENE_MENUE_OPTIONS 5
#define SCENE_MENUE_SINGLEPLAYER 6
#define SCENE_MENUE_MULTYPLAYER 7
#define SCENE_OPTIONS_SOUND 8
#define SCENE_OPTIONS_GRAPHIC 9
#define SCENE_OPTIONS_TEAM 10

//---UnitTests---
#define SCENE_XML_TEST 100
#define SCENE_ENGINE_TEST 101
#define SCENE_SOUND_TEST 102
#define SCENE_LEVEL_TEST 103
#define SCENE_PHYSIC_TEST 104

#define VERSION 1

class cGlobals
{

public:

	cGlobals();
	~cGlobals();
	char *CombiStr(char *sPath);
	
private:

	char sApplicationPath[255];
	char sBuffer[255];

};