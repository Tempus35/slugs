#pragma once

#include "sprite.h"
#include "animation.h"
#include "cFont.h"
#include "xml.h"
#include "cSFX.h"
#include "resourcemanager.h"
#include "world.h"
#include "slug.h"
#include "fxmanager.h"
#include "fx.h"
#include "renderer.h"
#include "camera.h"

#define CAMERA_MOVE_BORDER		20			// Pixel distance from the sides of the window in which the cursor causes the camera to move
#define CAMERA_MOVE_SHIFT		5.0f		// Movement speed
// #define CAMERA_EDGE_MOVE					// Move the camera when the cursor is at the window edge?

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
	
	Camera camera;
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