// +PB

#pragma once

#include "singleton.h"
#include "vector2.h"
#include "terrain.h"
#include "clouds.h"
#include "water.h"
#include "gameobject.h"
#include "slugobject.h"
#include "fxmanager.h"
#include "sprite.h"
#include "resourcemanager.h"
#include "renderer.h"

#define WORLD_DEFAULT_GRAVITY		5000
#define WORLD_WATER_LINES			5
#define WORLD_WATER_MIN_SPEED		50.0f
#define WORLD_WATER_MAX_SPEED		100.0f
#define WORLD_CLOUDS_MIN			5
#define WORLD_CLOUDS_MAX			20
#define WORLD_CLOUD_LAYERS			3
#define WORLD_EXPLOSION_BORDER		6
#define WORLD_EXPLOSION_MULTIPLIER	5.0f

#define WORLD_CROSSHAIR_DISTANCE	60
#define WORLD_ARROW_DISTANCE		75

class World : public Singleton<World>
{

friend class Singleton<World>;

private:

	Terrain* terrain;						// Pointer to terrain instance			

	Vector2 gravity;						// Gravity vector
	Vector2 wind;							// Wind vector

	list<GameObject*> objects;				// Doubly linked list for object storage
	vector<GameObject*> pendingObjects;

	Water* water[WORLD_WATER_LINES];
	Clouds* clouds;
	Sprite backgroundSprites[2];

	GameObject* selectedObject;

	Sprite crosshairSprite, arrowSprite;

	//
	// Initialization
	//	
	~World();
	World();

public:

	//
	// Setup
	//

	void Build(int width, int height, TextureBuffer* groundTexture, TextureBuffer* overTexture, TextureBuffer* underTexture, ImageResource* waterImage);
	void Regenerate();

	//
	// Simulation
	//

	void Update(float elapsedTime);
	void AddObject(GameObject* object);
	void AddCreatedObject(GameObject* object);
	void RemoveAllObjects();
void SetWaterColor(Color& color);
	void SetBackground(ImageResource* farImage, ImageResource* nearImage);
	void CameraMoved(sf::Vector2<float> newPosition);
	Vector2 ToWorldCoordinates(sf::View* camera, int screenWidth, int screenHeight, int screenX, int screenY);
	GameObject* SelectObjectAtPosition(Vector2 point);
	void SimulateExplosion(int x, int y, int strength);

	//
	// Rendering
	//

	void Render();

	//
	// Accessors
	//

	Vector2 Gravity();
	Vector2 Wind();
	unsigned int Seed();
	int WidthInPixels();
	int HeightInPixels();
	void SetGravity(Vector2 newGravity);
	void SetWind(Vector2 newWind);
	GameObject* SelectedObject();	

	//
	// Debug
	//

	void DestroyTerrain(int x, int y);

};