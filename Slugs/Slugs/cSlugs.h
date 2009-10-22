#pragma once

#include "sprite.h"
#include "animation.h"
#include "cFont.h"
#include "cXML.h"
#include "cSFX.h"
#include "resourcemanager.h"
#include "world.h"
#include "slugobject.h"
#include "fxmanager.h"
#include "fx.h"
#include "renderer.h"

#define CAMERA_MOVE_BORDER 20
#define CAMERA_MOVE_SHIFT 5.0f
// #define CAMERA_EDGE_MOVE

class cSlugs 
{
public:
	cSlugs();
	~cSlugs();

	bool bInit();
	void bUpdate();
	void bRender();
	void Clear();
	bool bInput(sf::Event *myEvent);

	// +PB
	void UpdateScene(float elapsedTime);
	void InitLevelTest();
	void Shutdown();
	//----

private:
	void Game();
	void GraphicTest();
	void SoundTest();
	void XMLTest();
	void LevelTest();
	void PhysicTest();
	void Menu();
	
	void UpdateFPS(float elapsedTime);
	void DrawMainMenue();
	void DrawGame();

	bool bPushed;
	bool bPushedLeft;
	bool bRunning;

	char buffer[300];
	char sFPS[25];
	bool bFullscreen;

	int iScreenWidth;
	int	iScreenHeight;
	int	iScene;
	int	iGroundObject;
	int frame;
	float fps;
	float time;
	int iPlayerIndex;

	int	iMouseX, lastMouseX;
	int	iMouseY, lastMouseY;
	bool rmbDown;

	sf::Clock clock;
	
	sf::View *cam;
	bool lockCameraToLevel;

	//Global Class
	cGlobals global;

	//XML
	cXML			config;

	//Fonts
	cFont			font;
	cFont			font2;
	cFont			font3;

	//Sounds
	cSFX			sfx;
};