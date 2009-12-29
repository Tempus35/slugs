#pragma once

#include <vector>

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
#include "pickup.h"

#define WORLD_DEFAULT_GRAVITY		-1000.0f
#define WORLD_WATER_LINES			5
#define WORLD_WATER_MIN_SPEED		50.0f
#define WORLD_WATER_MAX_SPEED		100.0f
#define WORLD_CLOUDS_MIN			5
#define WORLD_CLOUDS_MAX			20
#define WORLD_CLOUD_LAYERS			3
#define WORLD_EXPLOSION_BORDER		6

#define WORLD_ARROW_DISTANCE		75

class World
{

private:

	struct DeferredExplosion
	{

		Vec2f				position;
		ExplosionData		data;

		DeferredExplosion(const Vec2f& _position, const ExplosionData& _data) : position(_position), data(_data) {}

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
	bool									crosshairVisible;

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

	bool Update(float elapsedTime);
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
	void SimulateExplosion(const Vec2f& position, const ExplosionData& data);

	// Defers and explosion for processing later, used by the shotgun and other weapons
	void DeferExplosion(const Vec2f& position, const ExplosionData& data);

	// Processes all deferred explosions
	void SimulateExplosions();

	// Returns true if an object or the terrain lies within the circle
	bool GetCircleIntersection(const Vec2f& center, float radius);

	// Finds an intersection with the world along a ray
	Intersection GetRayIntersection(const Vec2f& start, const Vec2f& direction, Object* ignore, float range = Math::INFINITY);

	// Finds an intersection with the world along a line
	Intersection GetLineIntersection(const Vec2f& start, const Vec2f& end, Object* ignore);

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

	// Gets the nearest object of a given type to an object
	Object* GetNearestObject(Object* object, ObjectType type);

	// Gets all objects within a given radius of an object
	void GetObjectsNear(std::vector<Object*>& list, Object* object, float radius, ObjectType type = ObjectType_Any);

	// Gets all objects within a given radius of a point
	void GetObjectsNear(std::vector<Object*>& list, const Vec2f& point, float radius, ObjectType type = ObjectType_Any, Object* ignore= NULL);

	// Returns true if a direct line of sight exists between two objects
	bool ObjectCanSee(Object* from, Object* to);

	// Returns true if an object can hit another with a parabolic weapon
	bool ObjectCanSeeParabolic(Object* from, Object* to, Vec2f& optimalDirection, float& optimalSpeed);

	// Sets the position of the global crosshair sprite
	void SetCrosshairPosition(const Vec2f& position);

	// Enables or disables rendering the crosshair sprite
	void SetCrosshairVisible(bool state);

	// Creates and drops a random crate
	void DropCrate();

};