#pragma once

#include "fastmath.h"
#include "random.h"
#include "texturebuffer.h"
#include "vector2.h"
#include "color.h"
#include "renderer.h"

//
// Definitions
//

// #define PB_TERRAIN_DEBUG

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

	Vector2 position;
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

	vector<Vector2> points;

	TextureBuffer* textureBuffer;

	TextureBuffer* art[3];

	vector<DirtyRect> dirtyRects;

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

	//
	// Modification
	//

	void ClearSquare(int x0, int y0, int x1, int y1);
	void ClearCircle(int x, int y, int radius);
	void ClearCircle(int x, int y, int radius, int border);

	//
	// Generation
	//

	bool Generate(unsigned int seed);
	bool Generate(float landmass, float smoothness, float chaosity, float platformarity, float cavernacity, unsigned int seed = 0); 

	//
	// Collision detection
	//

	bool PointCollision(int x, int y);
	bool RowCollision(int centerX, int centerY, int width, int height, bool above);
	bool ColumnCollision(int centerX, int centerY, int width, int height, bool right);
	bool SquareCollision(int centerX, int centerY, int width, int height);
	bool SquareCollisionIterated(int fromX, int fromY, int toX, int toY, int width, int height, Vector2* collisionPosition);
	bool LineCollision(int x0, int y0, int x1, int y1, Vector2* collisionPosition);
	bool CircleCollision(int centerX, int centerY, int radius);
	Vector2 NormalAtPoint(int x, int y, float* angle);
	float AngleAtPoint(int x, int y);
	bool Contains(float x, float y);

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

	// Finds the first intersection between a ray and the terrain
	bool RayIntersection(const Vector2& start, const Vector2& direction, float maxRange, Vector2& collisionPos);

};