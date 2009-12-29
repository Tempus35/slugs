#pragma once

#include "debug.h"
#include "fastmath.h"
#include "random.h"
#include "texturebuffer.h"
#include "vec2.h"
#include "color.h"
#include "renderer.h"
#include "box.h"
#include "imageresource.h"
#include "sprite.h"

//
// Definitions
//

#define TERRAIN_BLOCK_SIZE					1024

#define TERRAIN_SMOOTHNESS_MIN				5.0f
#define TERRAIN_SMOOTHNESS_FACTOR			45.0f
#define TERRAIN_CHAOSITY_BASE_ANGLE			40.0f
#define TERRAIN_CHAOSITY_ADDITIONAL_ANGLE	40.0f
#define TERRAIN_MIN_HEIGHT					0.05f
#define TERRAIN_MAX_HEIGHT					0.75f
#define TERRAIN_MAX_BACK_STEPS				6
#define TERRAIN_PLATFORMARITY_BASE			30.0f
#define TERRAIN_PLATFORMARITY_FACTOR		70.0f
#define TERRAIN_MAX_CAVERNACITY 			100
#define TERRAIN_CAVERN_MIN_RADIUS			20.0f
#define TERRAIN_CAVERN_MAX_RADIUS			150.0f
#define TERRAIN_CAVERN_MAX_CONTINUATION		6
#define TERRAIN_CAVERN_CONTINUATION_CHANCE	1.0f
#define TERRAIN_CAVERN_SMOOTHNESS_MIN		20.0f
#define TERRAIN_CAVERN_SMOOTHNESS_FACTOR	30.0f
#define TERRAIN_CAVERN_CHAOSITY_FACTOR		0.4f
#define TERRAIN_CAVERN_EXPANSION_FACTOR		1.5f

/*
	enum TerrainAlpha
	Enumeration of alpha byte states
*/

enum TerrainAlpha
{

	TERRAINALPHA_EMPTY				= 0x00,
	TERRAINALPHA_TEMPORARY			= 0xFB,
	TERRAINALPHA_OVER				= 0xFC,
	TERRAINALPHA_UNDER				= 0xFD,
	TERRAINALPHA_EDGE				= 0xFE,
	TERRAINALPHA_FILLED				= 0xFF,
	
};

/*
	enum TerrainTrace
	Enumeration of terrain generation direction flags
*/

enum TerrainTrace
{

	TERRAINTRACE_NONE = 0,
	TERRAINTRACE_RIGHT = 1,
	TERRAINTRACE_LEFT = 2,
	TERRAINTRACE_UP = 4,
	TERRAINTRACE_DOWN = 8,

};

/*
	struct DirtyRect
*/

struct DirtyRect
{

	int left, top, right, bottom;

	DirtyRect(int l, int t, int r, int b)
	{

		left = l;
		top = t;
		right = r;
		bottom = b;

	}

};

/*
	class TerrainBlock
	A piece of terrain TERRAIN_BLOCK_SIZE^2 in size
*/

class TerrainBlock
{

private:

	Vec2i position;
	bool empty;
	ImageResource* imageResource;
	Sprite sprite;

public:

	//
	// Initialization
	//

	TerrainBlock();
	~TerrainBlock();

	//
	// Management
	//

	void Setup(int x, int y);
	void Update(TextureBuffer* buffer, int x0, int y0, int x1, int y1);
	void Update(TextureBuffer* buffer, DirtyRect& r);

	//
	// Accessors
	//

	Sprite& Sprite();
	bool IsEmpty();

	void SetEmpty(bool state);

};

/*
	class Terrain
	Manages a terrain including the image and collision data
*/

class Terrain
{

private:


	TerrainBlock** blocks;
	unsigned int genSeed;
	int width, height;
	int numBlocks;

	std::vector<Vec2f> points;

	TextureBuffer* textureBuffer;

	TextureBuffer* art[3];

	std::vector<DirtyRect> dirtyRects;

	//
	// Internals
	//

	void ProcessPoints();
	void FillColumn(int column, int row);
	void ClearColumn(int column, int row);
	void ClearColumnDrawUnder(int column, int row);
	void CreateCavern(int centerX, int centerY, float smoothness, float chaosity, int subLevel);
	void AddImageObject(int x, int y);
	void MarkEdge(int index, byte r = 0x00, byte g = 0x00, byte b = 0x00);

public:

	//
	// Initialization
	//

	Terrain(int widthInBlocks, int heightInBlocks);
	~Terrain();
	bool IsValid();

	//
	// Assets
	//

	void SetArt(TextureBuffer* ground, TextureBuffer* over, TextureBuffer* under);

	// Removes a circular section of the terrain
	void ClearCircle(const Vec2f& position, float radius, float border);

	//
	// Generation
	//

	bool Generate(unsigned int seed);
	bool Generate(float landmass, float smoothness, float chaosity, float platformarity, float cavernacity, unsigned int seed = 0); 

	//
	// Collision detection
	//

	bool Contains(float x, float y);
	bool ContainsOpenTop(float x, float y);

	//
	// Rendering
	//

	void Render();

	//
	// Accessors
	//

	int Seed();
	int Width();
	int Height();
	int NumBlocks();
	int WidthInPixels();
	int HeightInPixels();

	//
	// Debugging
	//

	void BufferAll();
	void BufferDirty();

	// Tests a point in the terrainbuffer for a collision
	bool PointCollision(float x, float y);

	// Tests a circle for an intersection with the terrain
	bool CircleCollision(const Vec2f& center, float radius);

	// Tests a row of the terrainbuffer for a collision
	bool RowCollision(float y, float startX, float endX);

	// Tests a column of the terrainbuffer for a collision
	bool ColumnCollision(float x, float startY, float endY);

	// Tests a box for collision with the terrain
	bool BoxCollision(float centerX, float centerY, float width, float height, Vec2f& collisionPos);

	// Tests a box for collision with the terrain as it moves betwene two points
	bool BoxCollisionIterated(float fromX, float fromY, float toX, float toY, float width, float height, Vec2f& collisionPosition, Vec2f& freePosition);

	// Finds the first intersection between a ray and the terrain
	bool RayIntersection(const Vec2f& start, const Vec2f& direction, float maxRange, Vec2f& collisionPos);

	// Calculates the terrain normal over a box
	Vec2f GetNormalForBox(float centerX, float centerY, float width, float height);

	// Calculates the highest height over a box
	float GetHeightForBox(const Boxf& box);

	// Gets the terrain height below a point
	float GetHeightAt(const Vec2f& position);

	// Gets the location of a free area on the terrain
	Vec2f GetSpawnPoint();

};