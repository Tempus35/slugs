#pragma once

#include "vec2.h"
#include "terrain.h"
#include "clouds.h"
#include "water.h"
#include "object.h"
#include "slug.h"
#include "fxmanager.h"
#include "sprite.h"
#include "resourcemanager.h"
#include "renderer.h"
#include "intersection.h"
#include "circle.h"

#define WORLD_DEFAULT_GRAVITY		-5000
#define WORLD_WATER_LINES			5
#define WORLD_WATER_MIN_SPEED		50.0f
#define WORLD_WATER_MAX_SPEED		100.0f
#define WORLD_CLOUDS_MIN			5
#define WORLD_CLOUDS_MAX			20
#define WORLD_CLOUD_LAYERS			3
#define WORLD_EXPLOSION_BORDER		6

#define WORLD_CROSSHAIR_DISTANCE	60
#define WORLD_ARROW_DISTANCE		75

class World
{

private:

	struct DeferredExplosion
	{

		float x, y;
		float strength;
		float forceMultiplier;

		DeferredExplosion(float _x, float _y, float _strength, float _forceMultiplier) : x(_x), y(_y), strength(_strength), forceMultiplier(_forceMultiplier) {}

	};

public:

	enum IntersectionType
	{

		IntersectionType_None,
		IntersectionType_Terrain,
		IntersectionType_Object,

	};

private:

	Terrain*								terrain;						// Pointer to terrain instance			

	Vec2f									gravity;						// Gravity vector
	Vec2f									wind;							// Wind vector

	list<Object*>							objects;						// List of objects in the world
	
	vector<Circle>							areaBlocks;						// List of blocked areas (used when generating spawn points)

	vector<Object*>							pendingObjects;					// List of objects created and waiting to be added
	vector<DeferredExplosion>				deferredExplosions;				// List of deferred explosions awaiting processing


	Water*									water[WORLD_WATER_LINES];
	Clouds*									clouds;
	Sprite									backgroundSprites[2];

	Object*									selectedObject;

	Sprite									crosshairSprite, arrowSprite;

public:

	// Initialization
	World();
	~World();

	//
	// Setup
	//

	void Build(int width, int height, TextureBuffer* groundTexture, TextureBuffer* overTexture, TextureBuffer* underTexture, ImageResource* waterImage);
	void Regenerate();

	//
	// Simulation
	//

	void Update(float elapsedTime);
	void AddObject(Object* object);
	void AddCreatedObject(Object* object);
	void RemoveAllObjects();
	void SetWaterColor(Color& color);
	void SetBackground(ImageResource* farImage, ImageResource* nearImage);
	void CameraMoved(const Vec2f& newPosition);
	
	Object* SelectObjectAtPosition(const Vec2f& position);
	void SelectObject(Object* object);

	Object* GetObjectAtPosition(const Vec2f& position);

	//
	// Rendering
	//

	void Render();

	//
	// Accessors
	//

	Vec2f Gravity();
	Vec2f Wind();
	unsigned int Seed();
	int WidthInPixels();
	int HeightInPixels();
	void SetGravity(Vec2f newGravity);
	void SetWind(Vec2f newWind);
	Object* SelectedObject();	

	// Method used to simulate an explosion, destroys terrain and damages objects
	void SimulateExplosion(float x, float y, float strength, float forceMultiplier = 1.25f);

	// Defers and explosion for processing later, used by the shotgun and other weapons
	void DeferExplosion(float x, float y, float strength, float forceMultiplier = 1.25f);

	// Processes all deferred explosions
	void SimulateExplosions();

	// Finds a collision along a ray
	IntersectionType GetRayIntersection(const Vec2f& start, const Vec2f& direction, Vec2f& collisionPos, Object* ignore);

	// Gets the terrain normal at a point
	Vec2f GetNormal(const Vec2f& position) const;

	// Gets the terrain normal evaluated over a box
	Vec2f GetNormalForBox(float centerX, float centerY, float width, float height) const;

	// Gets a pointer to the terrain object
	Terrain* GetTerrain() const;

	// Creates a set of valid spawn points
	void GetSpawnPoints(std::vector<Vec2f>& list, int count);

	// Gets a free spawn location and blocks the area from being chosen again
	Vec2f GetSpawnPointAndBlock(float blockRadius);

	// Checks a point to see if it is blocked
	bool IsBlocked(const Vec2f& position);

	// Removes all area blocks from the world
	void ClearBlocks();

};