#include "terrain.h"

/*
	class TerrainBlock
*/

//
// Initialization
//

TerrainBlock::TerrainBlock()
{

	empty = false;
	imageResource = NULL;

}

TerrainBlock::~TerrainBlock()
{

}

//
// Management
//

void TerrainBlock::Setup(int x, int y)
{

	// Create the sf::image if we haven't already
	if (!imageResource)
	{

		imageResource = new ImageResource(TERRAIN_BLOCK_SIZE, TERRAIN_BLOCK_SIZE, false);

		position = Vector2(x, y);

		sprite.SetImage(imageResource);
		sprite.SetPosition((float)x, -(float)y - TERRAIN_BLOCK_SIZE);

	}

}

void TerrainBlock::Update(TextureBuffer* buffer, int x0, int y0, int x1, int y1)
{

	if (!empty)
	{

		sf::Image& image = imageResource->Image();

		// Couldn't find a buffer with stride function for sf::image, so we have to copy pixels
		Color* ptr = (Color*)buffer->Data((int)position.x + x0, (int)position.y + y0);
		int yAdvance = buffer->Width() - (x1 - x0);
		sf::Color c;
		for (int i = y0; i < y1; ++ i)
		{

			for (int j = x0; j < x1; ++ j)
			{

				c.r = ptr->r;
				c.g = ptr->g;
				c.b = ptr->b;
				c.a = ptr->a;

				image.SetPixel(j, TERRAIN_BLOCK_SIZE - 1 - i, c);

				ptr ++;

			}

			ptr += yAdvance;

		}

	}

}

void TerrainBlock::Update(TextureBuffer* buffer, DirtyRect& r)
{

	int x[2], y[2];
	x[0] = max((int)position.x, r.left);
	y[0] = max((int)position.y, r.top);
	x[1] = min((int)position.x + TERRAIN_BLOCK_SIZE, r.right);
	y[1] = min((int)position.y + TERRAIN_BLOCK_SIZE, r.bottom);

	if ((x[0] != x[1]) || (y[0] != y[1]))
		Update(buffer, x[0] - (int)position.x, y[0] - (int)position.y, x[1] - (int)position.x, y[1] - (int)position.y);

}

//
// Accessors
//

Sprite& TerrainBlock::Sprite()
{

	return sprite;

}

bool TerrainBlock::IsEmpty()
{

	return empty;

}

void TerrainBlock::SetEmpty(bool state)
{

	empty = true;

}

/*
	class Terrain
*/

//
// Initialization
//

Terrain::Terrain(int widthInBlocks, int heightInBlocks)
{

	width = widthInBlocks;
	height = heightInBlocks;
	
	// Create texture buffer
	textureBuffer = new TextureBuffer(width * TERRAIN_BLOCK_SIZE, height * TERRAIN_BLOCK_SIZE, 4);

	if (textureBuffer)
	{

		// Create block buffer
		numBlocks = width * height;
		blocks = new TerrainBlock*[numBlocks];

		if (blocks)
		{

			for (int i = 0; i < numBlocks; ++ i)
				blocks[i] = new TerrainBlock();

		}

	}

}

Terrain::~Terrain()
{

	// Delete blocks
	if (blocks)
	{

		for (int i = 0; i < numBlocks; ++ i)
		{

			if (blocks[i])
				delete blocks[i];

		}

		delete blocks;

	}

	// Delete texture buffer
	if (textureBuffer)
		delete textureBuffer;

}

bool Terrain::IsValid()
{

	return ((textureBuffer != NULL) && (blocks != NULL));

}

//
// Internals
//

void Terrain::ProcessPoints()
{

	int last[2], p[2], lastDirection, nextDirection, currentDirection;
	last[0] = -1;
	last[1] = -1;
	lastDirection = TERRAINTRACE_NONE;

	// Determine direction of the first segment
	currentDirection = TERRAINTRACE_NONE;
	Vector2 f = points[1] - points[0];

	if (f.x >= 0)
		currentDirection |= TERRAINTRACE_RIGHT;
	else
		currentDirection |= TERRAINTRACE_LEFT;

	if (f.y >= 0)
		currentDirection |= TERRAINTRACE_UP;
	else
		currentDirection |= TERRAINTRACE_DOWN;

	// Loop through segments
	for (unsigned int i = 1; i < points.size(); ++ i)
	{

		// Determine direction of the next segment
		nextDirection = TERRAINTRACE_NONE;

		if (i < points.size() - 1)
		{

			Vector2 nextV = points[i + 1] - points[i];

			if (i < points.size() - 1)
			{

				if (nextV.x >= 0)
					nextDirection |= TERRAINTRACE_RIGHT;
				else
					nextDirection |= TERRAINTRACE_LEFT;

				if (nextV.y >= 0)
					nextDirection |= TERRAINTRACE_UP;
				else
					nextDirection |= TERRAINTRACE_DOWN;

			}

		}

		// Get segment vector
		Vector2 v = points[i] - points[i - 1];
		float distance = VectorLength(v);
		VectorNormalize(&v);

		// Set start point
		float lerp = 0.0f;
		Vector2 pt = points[i - 1];

		bool firstPoint = true;
		bool lastPoint = false;

		// Interpolate across segment
		while (lerp < distance)
		{

			// Get integer position
			p[0] = (int)pt.x;
			p[1] = (int)pt.y;

			// If we didn't move to the next pixel, don't do any more work
			if ((p[0] != last[0]) || (p[1] != last[1]))
			{

				int index = textureBuffer->PositionToIndex(p[0], clamp(p[1], 0, textureBuffer->Height() - 1));

				if (index)
				{

					if (currentDirection & TERRAINTRACE_RIGHT)
					{

						if (firstPoint)
						{

							if (lastDirection & TERRAINTRACE_LEFT)
							{

								if (currentDirection & TERRAINTRACE_DOWN)
								{
									
									FillColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0xFF, 0x00);

								}
								else
									MarkEdge(index, 0xFF, 0xFF, 0x00);


							}
							else
							{

								FillColumn(p[0], p[1]);
								MarkEdge(index, 0x00, 0xFF, 0x00);

							}

						}
						else if (lastPoint)
						{

							if (nextDirection & TERRAINTRACE_LEFT)
							{

								if (currentDirection & TERRAINTRACE_UP)
								{

									FillColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0xFF, 0x00);

								}
								else
								{

									ClearColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0x00, 0xFF);

								}

							}
							else
							{

								FillColumn(p[0], p[1]);
								MarkEdge(index, 0x00, 0xFF, 0x00);

							}

						}
						else
						{

							FillColumn(p[0], p[1]);
							MarkEdge(index, 0x00, 0xFF, 0x00);

						}

					}
					else
					{

						if (firstPoint)
						{

							if (lastDirection & TERRAINTRACE_RIGHT)
							{

								if (currentDirection & TERRAINTRACE_DOWN)
								{
									ClearColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0x00, 0xFF);
								}
								else
								{
									FillColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0xFF, 0x00);
								}

							}
							else
							{
		
								if (currentDirection & TERRAINTRACE_DOWN)
								{

									ClearColumnDrawUnder(p[0], p[1]);
									MarkEdge(index, 0x00, 0x00, 0xFF);

								}
								else
									MarkEdge(index, 0xFF, 0x00, 0x00);

							}

						}
						else if (lastPoint)
						{

							if (nextDirection & TERRAINTRACE_RIGHT)
							{

								if (currentDirection & TERRAINTRACE_DOWN)
								{

									FillColumn(p[0], p[1]);
									MarkEdge(index, 0x00, 0xFF, 0x00);

								}
								else
									MarkEdge(index, 0xFF, 0xFF, 0x00);

							}
							else
							{

								if (currentDirection & TERRAINTRACE_DOWN)
								{

									ClearColumnDrawUnder(p[0], p[1]);
									MarkEdge(index, 0x00, 0x00, 0xFF);

								}
								else
									MarkEdge(index, 0xFF, 0x00, 0x00);

							}
							
						}
						else
						{

							if (currentDirection & TERRAINTRACE_DOWN)
							{

								ClearColumnDrawUnder(p[0], p[1]);
								MarkEdge(index, 0x00, 0x00, 0xFF);

							}
							else
								MarkEdge(index, 0xFF, 0x00, 0x00);

						}

					}	

				}

			}

			// Store pixel location as last pixel location
			last[0] = p[0];
			last[1] = p[1];

			// Next pixel
			pt.x += v.x;
			pt.y += v.y;
			lerp += 1.0f;

			if (lerp + 1.0f >= distance)
				lastPoint = true;

			if (firstPoint)
				firstPoint = false;

		}

		// Moving to the next segment, update directions
		lastDirection = currentDirection;
		currentDirection = nextDirection;

	}

	// We are done with our points
	points.clear();

}

void Terrain::FillColumn(int column, int row)
{

	int index = textureBuffer->PositionToIndex(column, min(row, textureBuffer->Height() - 1));
	Color* data = (Color*)textureBuffer->Data();

	if (index)
	{

		int groundWidth = art[0]->Width();
		int groundHeight = art[0]->Height();
		int overWidth = art[1]->Width();
		int overHeight = art[1]->Height();
		int underWidth = art[2]->Width();
		int underHeight = art[2]->Height();

		// Find the end point
		int idx = index;
		int under = 0;
		while (idx >= 0)
		{

			if ((idx < 0) || (idx >= textureBuffer->dataSize))
				throw("EXC");

			if (data[idx].a == TERRAINALPHA_EDGE)
				break;

			idx -= textureBuffer->Width();
			under ++;

		}

		if (idx < 0)
			under = 1000000;
		else
			under -= underHeight;

		int filled = 0;
		while (index >= 0)
		{

			if ((index < 0) || (index >= textureBuffer->dataSize))
				throw("EXC");

			if (data[index].a == TERRAINALPHA_EDGE)
				break;

			if (filled < overHeight)
			{

				// Over texture
				Color* color = &((Color*)art[1]->Data())[column % overWidth + filled * overWidth];
				data[index].r = color->r;
				data[index].g = color->g;
				data[index].b = color->b;
				data[index].a = TERRAINALPHA_OVER;

			}
			else if (filled > under)
			{

				// Under texture
				Color* color = &((Color*)art[2]->Data())[column % underWidth + (filled - under) * underWidth];
				data[index].r = color->r;
				data[index].g = color->g;
				data[index].b = color->b;
				data[index].a = TERRAINALPHA_UNDER;

			}
			else
			{

				// Main texture
				Color* color = &((Color*)art[0]->Data())[column % groundWidth + ((row - filled) % groundHeight) * groundWidth];
				data[index].r = color->r;
				data[index].g = color->g;
				data[index].b = color->b;
				data[index].a = TERRAINALPHA_FILLED;

			}

			filled ++;

			index -= textureBuffer->Width();

		}

	}

}

void Terrain::ClearColumn(int column, int row)
{

	int index = textureBuffer->PositionToIndex(column, min(row, textureBuffer->Height() - 1));
	Color* data = (Color*)textureBuffer->Data();

	if (data)
	{

		while (index >= 0)
		{

			if ((index < 0) || (index >= textureBuffer->dataSize))
				throw("EXC");

			if (data[index].a == TERRAINALPHA_EDGE)
				break;

			data[index].r = data[index].g = data[index].b = data[index].a = TERRAINALPHA_EMPTY;

			index -= textureBuffer->Width();

		}

	}

}

void Terrain::ClearColumnDrawUnder(int column, int row)
{

	int index = textureBuffer->PositionToIndex(column, min(row, textureBuffer->Height() - 1));
	Color* data = (Color*)textureBuffer->Data();

	if (data)
	{

		// Draw under texture above
		int r = row + 1;
		int idx;
		int changed = 0;
		int underWidth = art[2]->Width();
		int underHeight = art[2]->Height();

		while (r < textureBuffer->Height() - 1)
		{

			idx = textureBuffer->PositionToIndex(column, r);

			if (idx != -1)
			{

				if ((idx < 0) || (idx >= textureBuffer->dataSize))
					throw("EXC");

				if ((data[idx].a == TERRAINALPHA_OVER) || (data[idx].a == TERRAINALPHA_EMPTY))
					break;

				Color* color = &((Color*)art[2]->Data())[column % underWidth + changed * underWidth];
				data[idx].r = color->r;
				data[idx].g = color->g;
				data[idx].b = color->b;
				data[idx].a = TERRAINALPHA_UNDER;

				changed ++;

				if (changed == underHeight)
					break;

			}

			r ++;

		}

		// Clear
		while (index >= 0)
		{

			if ((index < 0) || (index >= textureBuffer->dataSize))
				throw("EXC");

			if (data[index].a == TERRAINALPHA_EDGE)
				break;

			data[index].r = data[index].g = data[index].b = data[index].a = TERRAINALPHA_EMPTY;

			index -= textureBuffer->Width();

		}

	}

}

void Terrain::CreateCavern(int centerX, int centerY, float smoothness, float chaosity, int subLevel)
{

	Color* data = (Color*)textureBuffer->Data(centerX, centerY);

	// Check to see if the point is on the terrain (initial cavern only)
	if ((subLevel == 0) && ((data) && (data->a != TERRAINALPHA_EMPTY)))
		return;

	float averageRadius = Random::RandomFloat(TERRAIN_CAVERN_MIN_RADIUS, TERRAIN_CAVERN_MAX_RADIUS);
	float averageCircumferance = TWO_PI * averageRadius;
	float distanceStep = TERRAIN_CAVERN_SMOOTHNESS_MIN + (1.0f - smoothness) * TERRAIN_CAVERN_SMOOTHNESS_FACTOR;
	int numPoints = (int)(averageCircumferance / distanceStep);
	float angleStep = TWO_PI / (float)(numPoints - 1);
	float variance = TERRAIN_CAVERN_CHAOSITY_FACTOR * chaosity * averageRadius;
	float currentRadius = averageRadius;
	
	int largestBorder;

	if (art[1]->Height() > art[2]->Height())
		largestBorder = art[1]->Height();
	else
		largestBorder = art[2]->Height();

	int maxRadius = (int)(averageRadius + variance + largestBorder);

	int x[2], y[2];
	if (textureBuffer->Intersection(centerX - maxRadius, centerY - maxRadius, centerX + maxRadius, centerY + maxRadius, x, y))
	{

		float* radii = new float[numPoints];

		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);
		int yAdvance = textureBuffer->Width() - (x[1] - x[0]);
		float radiusSquared;
		int dx, dy;

		// Create radii table
		for (int i = 0; i < numPoints - 1; ++ i)
		{

			if (currentRadius > averageRadius)
				currentRadius -= Random::RandomFloat(0.0f, variance);
			else
				currentRadius += Random::RandomFloat(0.0f, variance);

			radii[i] = currentRadius;

		}

		radii[numPoints - 1] = radii[0];

		// Clear
		int first, second;
		float lerp;
		for (int i = y[0]; i < y[1]; ++ i)
		{

			for (int j = x[0]; j < x[1]; ++ j)
			{

				dx = j - centerX;
				dy = i - centerY;

				float angle = atan2f((float)dy, (float)dx) + PI;
				first = (int)(floorf(angle / angleStep));
				second = first + 1;
				lerp = (angle - (first * angleStep)) / angleStep;

				currentRadius = radii[first] + (radii[second] - radii[first]) * lerp;
				radiusSquared = currentRadius * currentRadius;

				if (dx * dx + dy * dy < radiusSquared)
				{

					ptr->a = TERRAINALPHA_EMPTY;

				}

				ptr ++;

			}

			ptr += yAdvance;

		}

		delete[] radii;

		// Rescan for upper edges
		byte last;
		bool filling = false;
		int count;
		int halfway = y[0] + (y[1] - y[0]) / 2;
		for (int i = x[0]; i < x[1]; ++ i)
		{

			last = TERRAINALPHA_FILLED;
			count = 0;
			ptr = (Color*)textureBuffer->Data(i, halfway);

			for (int j = halfway; j < y[1]; ++ j)
			{

				if (ptr->a == TERRAINALPHA_FILLED)
				{

					if (last == TERRAINALPHA_EMPTY)
						filling = true;

				}
				else
				{

					filling = false;
					count = 0;

				}

				last = ptr->a;

				if (filling)
				{

					if (ptr->a == TERRAINALPHA_FILLED)
					{
							
						Color* color = &((Color*)art[2]->Data())[i % art[2]->Width() + count * art[2]->Width()];
						ptr->r = color->r;
						ptr->g = color->g;
						ptr->b = color->b;
						ptr->a = TERRAINALPHA_UNDER;

					}

					count ++;

					if (count >= art[2]->Height())
						filling = false;

				}
				
				ptr += textureBuffer->Width();

			}

		}

		// Rescan for lower edges
		for (int i = x[0]; i < x[1]; ++ i)
		{

			last = TERRAINALPHA_FILLED;
			count = 0;
			ptr = (Color*)textureBuffer->Data(i, halfway);

			for (int j = halfway; j < y[1]; ++ j)
			{

				if (ptr->a != TERRAINALPHA_EMPTY)
				{

					if (last == TERRAINALPHA_EMPTY)
						filling = true;

				}
				else
				{

					filling = false;
					count = 0;

				}

				last = ptr->a;

				if (filling)
				{
	
					Color* color = &((Color*)art[1]->Data())[i % art[1]->Width() + count * art[1]->Width()];
					ptr->r = color->r;
					ptr->g = color->g;
					ptr->b = color->b;
					ptr->a = TERRAINALPHA_OVER;

					count ++;

					if (count >= art[1]->Height())
						filling = false;

				}
				
				ptr -= textureBuffer->Width();

			}

		}

		// Recursively expand the cavern
		if (subLevel < TERRAIN_CAVERN_MAX_CONTINUATION)
		{

			float continuationChance = TERRAIN_CAVERN_CONTINUATION_CHANCE;
			for (int i = 0; i < subLevel; ++ i)
				continuationChance *= 0.5f;

			if (Random::RandomFloat() < continuationChance)
			{

				float angle = Random::RandomFloat(0, TWO_PI);
				int nextX = (int)(centerX + fastcosf(angle) * averageRadius * TERRAIN_CAVERN_EXPANSION_FACTOR);
				int nextY = (int)(centerY + fastsinf(angle) * averageRadius * TERRAIN_CAVERN_EXPANSION_FACTOR);
				CreateCavern(nextX, nextY, smoothness, chaosity, subLevel + 1);

			}

		}

	}

}

void Terrain::AddImageObject(int x, int y)
{

}

void Terrain::MarkEdge(int index, byte r, byte g, byte b)
{

	if (!textureBuffer)
		return;

	if ((index < 0) || (index >= textureBuffer->dataSize))
		return;

	Color* data = (Color*)textureBuffer->Data();

	if (data)
	{

		#ifdef PB_TERRAIN_DEBUG

			data[index].r = r;
			data[index].g = g;
			data[index].b = b;
			data[index].a = TERRAINALPHA_EDGE;
		
		#else

			data[index].a = TERRAINALPHA_EDGE;

		#endif

	}

}

void Terrain::BufferDirty()
{

	if (!IsValid())
		return;

	if (dirtyRects.size() > 0)
	{

		for (int y = 0; y < height; ++ y)
		{

			for (int x = 0; x < width; ++ x)
			{

				int blockIndex = x + y * width;
				for (unsigned int j = 0; j < dirtyRects.size(); ++ j)
				{

					// Determine intersection between block and dirty rect
					blocks[blockIndex]->Update(textureBuffer, dirtyRects[j]);
						
				}

			}

		}

		dirtyRects.clear();

	}

}

void Terrain::BufferAll()
{

	if (!IsValid())
		return;

	// Update all terrain blocks
	int index;
	for (int i = 0; i < height; ++ i)
	{

		for (int j = 0; j < width; ++ j)
		{

			index = j + i * width;

			if (blocks[index])
			{

				blocks[index]->Setup(j * TERRAIN_BLOCK_SIZE, i * TERRAIN_BLOCK_SIZE);
				blocks[index]->Update(textureBuffer, 0, 0, TERRAIN_BLOCK_SIZE, TERRAIN_BLOCK_SIZE);

			}

		}

	}

}

//
// Assets
//

void Terrain::SetArt(TextureBuffer* ground, TextureBuffer* over, TextureBuffer* under)
{

	art[0] = ground;
	art[1] = over;
	art[2] = under;

}

//
// Modification
//

void Terrain::ClearSquare(int x0, int y0, int x1, int y1)
{

	int x[2], y[2];

	// Clamp the area to the buffer
	if (textureBuffer->Intersection(x0, y0, x1, y1, x, y))
	{

		// Get pointer to data
		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);

		if (ptr)
		{

			// Clear the section
			int yAdvance = textureBuffer->Width() - (x[1] - x[0]);

			for (int i = y[0]; i < y[1]; ++ i)
			{

				for (int j = x[0]; j < x[1]; ++ j)
				{

					ptr->a = TERRAINALPHA_EMPTY;
					ptr ++;

				}

				ptr += yAdvance;

			}

		}

	}

}

void Terrain::ClearCircle(int centerX, int centerY, int radius)
{

	int x[2], y[2];

	// Clamp the enclosing square to the buffer
	if (textureBuffer->Intersection(centerX - radius, centerY - radius, centerX + radius, centerY + radius, x, y))
	{

		// Get pointer to data
		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);

		if (ptr)
		{

			// Clear the section
			int yAdvance = textureBuffer->Width() - (x[1] - x[0]);
			int radiusSquared = radius * radius;
			int dx, dy;

			for (int i = y[0]; i < y[1]; ++ i)
			{

				for (int j = x[0]; j < x[1]; ++ j)
				{

					dx = j - centerX;
					dy = i - centerY;

					// Restrict to within radius of circle
					if (dx * dx + dy * dy < radiusSquared)
						ptr->a = TERRAINALPHA_EMPTY;

					ptr ++;

				}

				ptr += yAdvance;

			}

		}

	}

}

void Terrain::ClearCircle(int centerX, int centerY, int radius, int border)
{

	int x[2], y[2];
	int maxRadius = radius + border;

	// Clamp the enclosing square to the buffer
	if (textureBuffer->Intersection(centerX - maxRadius, centerY - maxRadius, centerX + maxRadius, centerY + maxRadius, x, y))
	{

		// Set dirty rect
		dirtyRects.insert(dirtyRects.begin(), DirtyRect(x[0], y[0], x[1], y[1]));

		// Get pointer to data
		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);

		if (ptr)
		{

			// Clear the section
			int yAdvance = textureBuffer->Width() - (x[1] - x[0]);
			int radiusSquared = radius * radius;
			int maxRadiusSquared = maxRadius * maxRadius;
			int dx, dy, dsq;

			Color* color = (Color*)art[0]->Data();

			for (int i = y[0]; i < y[1]; ++ i)
			{

				for (int j = x[0]; j < x[1]; ++ j)
				{

					dx = j - centerX;
					dy = i - centerY;
					dsq = dx * dx + dy * dy;

					// Restrict to within radius of circle
					if (dsq < maxRadiusSquared)
					{

						if (dsq < radiusSquared)
							ptr->a = TERRAINALPHA_EMPTY;
						else if (ptr->a != TERRAINALPHA_EMPTY)
						{

							ptr->r = color->r;
							ptr->g = color->g;
							ptr->b = color->b;

						}

					}

					ptr ++;

				}

				ptr += yAdvance;

			}

		}

	}

}

//
// Collision detection
//

bool Terrain::PointCollision(int x, int y)
{

	Color* data = (Color*)textureBuffer->Data(x, y);
	
	return ((data != NULL) && (data->a != TERRAINALPHA_EMPTY));

}

bool Terrain::SquareCollision(int centerX, int centerY, int width, int height)
{

	// Clamp the area to the buffer
	int x[2], y[2];
	int halfWidth = width / 2;
	int halfHeight = height / 2;

	if (textureBuffer->Intersection(centerX - halfWidth, centerY - halfHeight, centerX + halfWidth, centerY + halfHeight, x, y))
	{

		// Get pointer to data
		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);

		if (ptr)
		{

			// Clear the section
			int yAdvance = textureBuffer->Width() - (x[1] - x[0]);

			for (int i = y[0]; i < y[1]; ++ i)
			{

				for (int j = x[0]; j < x[1]; ++ j)
				{

					// Check for collision
					if (ptr->a != TERRAINALPHA_EMPTY)
						return true;

					ptr ++;

				}

				ptr += yAdvance;

			}

		}

	}

	return false;

}

bool Terrain::SquareCollisionIterated(int fromX, int fromY, int toX, int toY, int width, int height, Vector2* collisionPosition)
{

	return SquareCollision(toX, toY, width, height);

}

bool Terrain::LineCollision(int x0, int y0, int x1, int y1, Vector2* collisionPosition)
{

	// Clamp the line to the buffer
	int x[2], y[2];

	if (textureBuffer->Intersection(x0, y0, x1, y1, x, y))
	{

		Vector2 v(x[1] - x[0], y[1] - y[0]);
		float distance = VectorLength(v);
		VectorNormalize(&v);
		float lerp = 0.0f;
		Vector2 p(x[0], y[0]);

		while (lerp < distance)
		{

			Color* data = (Color*)textureBuffer->Data((int)p.x, (int)p.y);

			if (data)
			{

				if (data->a != TERRAINALPHA_EMPTY)
				{
				
					if (collisionPosition)
					{

						(*collisionPosition).x = p.x;
						(*collisionPosition).y = p.y;

					}

					return true;

				}

			}

			p += v;
			lerp += 1.0f;

		}

	}

	return false;

}

bool Terrain::CircleCollision(int centerX, int centerY, int radius)
{

	int x[2], y[2];

	// Clamp the enclosing square to the buffer
	if (textureBuffer->Intersection(centerX - radius, centerY - radius, centerX + radius, centerY + radius, x, y))
	{

		// Get pointer to data
		Color* ptr = (Color*)textureBuffer->Data(x[0], y[0]);

		if (ptr)
		{

			// Clear the section
			int yAdvance = textureBuffer->Width() - (x[1] - x[0]);
			int radiusSquared = radius * radius;
			int dx, dy;

			for (int i = y[0]; i < y[1]; ++ i)
			{

				for (int j = x[0]; j < x[1]; ++ j)
				{

					dx = j - centerX;
					dy = i - centerY;

					// Restrict to within radius of circle
					if (dx * dx + dy * dy < radiusSquared)
					{

						// Check for collision
						if (ptr->a != TERRAINALPHA_EMPTY)
							return true;

					}

					ptr ++;

				}

				ptr += yAdvance;

			}

		}

	}

	return false;

}

float Terrain::AngleAtPoint(int x, int y)
{

	Color* data = (Color*)textureBuffer->Data(x, y);

	// Make sure pixel at x, y isn't empty
	if (data)
	{

		int pixels = 0;

		// Test upper pixel
		data = (Color*)textureBuffer->Data(x, y + 1);

		if (data->a != TERRAINALPHA_EMPTY)
			return PI_OVER_2;

		// Test upper right
		data = (Color*)textureBuffer->Data(x + 1, y + 1);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 1;

		// Test upper left
		data = (Color*)textureBuffer->Data(x - 1, y + 1);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 2;

		// Upper left and upper right gives average flat terrain
		if ((pixels & 2) && (pixels & 1))
			return 0.0f;

		// Test right
		data = (Color*)textureBuffer->Data(x + 1, y);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 4;

		// Test left
		data = (Color*)textureBuffer->Data(x - 1, y);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 8;

		if (pixels & 1)
		{

			// Upper right with left gives 30 degree slope
			if (pixels & 8)
				return 30.0f * PI_OVER_180;
			
			// Upper right without left gives 45 degree slope
			return 45.0f * PI_OVER_180;

		}
		else if (pixels & 2)
		{

			// Upper left with right gives -30 degree slope
			if (pixels & 4)
				return -30.0f * PI_OVER_180;

			// Upper left without right gives -45 degree slope
			return -45.0f * PI_OVER_180;

		}
		
		// Left or right without any upper pixel gives flat terrain
		if ((pixels & 8) || (pixels & 4))
			return 0.0f;

		// Test lower right
		data = (Color*)textureBuffer->Data(x + 1, y - 1);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 16;

		// Test lower left
		data = (Color*)textureBuffer->Data(x - 1, y - 1);

		if (data->a != TERRAINALPHA_EMPTY)
			pixels |= 32;

		// Lower right gives -45 degree slope
		if (pixels & 16)
			return -45.0f * PI_OVER_180;

		// Lower left give 45 degree slope
		if (pixels & 32)
			return 45.0f * PI_OVER_180;

		// Any other combination is flat
		return 0.0f;

	}

	return TWO_PI; // Point not on ground

}

Vector2 Terrain::NormalAtPoint(int x, int y, float* angle)
{

	Color* data = (Color*)textureBuffer->Data(x, y);

	Vector2 n(0, 0);
	if (data->a != TERRAINALPHA_EMPTY)
	{

		// Test surrounding pixels
		if (y > 0)
		{

			// Bottom
			data = (Color*)textureBuffer->Data(x, y - 1);

			if (data->a != TERRAINALPHA_EMPTY)
				n.y -= 1.0f;

			// Bottom Left
			data = (Color*)textureBuffer->Data(x - 1, y - 1);

			if (data->a != TERRAINALPHA_EMPTY)
			{
				n.x -= 1.0f;
				n.y -= 1.0f;
			}

			// Bottom Right
			data = (Color*)textureBuffer->Data(x + 1, y - 1);

			if (data->a != TERRAINALPHA_EMPTY)
			{
				n.x += 1.0f;
				n.y -= 1.0f;
			}


		}
		
		if (y < height - 1)
		{

			// Top
			data = (Color*)textureBuffer->Data(x, y + 1);

			if (data->a != TERRAINALPHA_EMPTY)
				n.y += 1.0f;

			// Top left
			data = (Color*)textureBuffer->Data(x - 1, y + 1);

			if (data->a != TERRAINALPHA_EMPTY)
			{
				n.x -= 1.0f;
				n.y += 1.0f;
			}

			// Top right
			data = (Color*)textureBuffer->Data(x + 1, y + 1);

			if (data->a != TERRAINALPHA_EMPTY)
			{
				n.x += 1.0f;
				n.y += 1.0f;
			}

		}

		if (x > 0)
		{

			// Left
			data = (Color*)textureBuffer->Data(x - 1, y);

			if (data->a != TERRAINALPHA_EMPTY)
				n.y -= 1.0f;

		}

		if (x < width - 1)
		{

			// Right
			data = (Color*)textureBuffer->Data(x + 1, y);

			if (data->a != TERRAINALPHA_EMPTY)
				n.y += 1.0f;

		}

		// Normalize vector
		VectorNormalize(&n);

	}

	// Calculate angle if needed
	if (angle)
		*angle = atan2f(n.y, n.x) + PI;
	
	return n;

}

bool Terrain::Contains(float x, float y)
{

	if ((x < 0) || (x >= textureBuffer->Width()) || (y < 0) || (y >= textureBuffer->Height()))
		return false;

	return true;

}

//
// Generation
//

bool Terrain::Generate(unsigned int seed)
{

	Random::SetSeed(seed);

	float landmass = Random::RandomFloat();
	float smoothness = Random::RandomFloat();
	float chaosity = Random::RandomFloat();
	float platformarity = Random::RandomFloat();
	float cavernacity = Random::RandomFloat();

	return Generate(landmass, smoothness, chaosity, platformarity, cavernacity, seed);

}

bool Terrain::Generate(float landmass, float smoothness, float chaosity, float platformarity, float cavernacity, unsigned int seed)
{

	// Make sure our resources initialized
	if (!IsValid())
		return false;

	// Zero the texture buffer
	textureBuffer->Fill(0x00);

	// Set random generator seed
	genSeed = Random::SetSeed(genSeed);

	// Generate terrain
	const int numPoints = 10000000;		// Absolute maximum number of points to consider
	float minDistance = TERRAIN_SMOOTHNESS_MIN + (1.0f - smoothness) * TERRAIN_SMOOTHNESS_FACTOR;	
	float maxDistance = 2 * minDistance;		

	// Set starting point
	Vector2 p = Vector2(0, (int)((float)textureBuffer->Height() * landmass * 0.5f));
	points.insert(points.end(), p);

	// Set angles and separation
	float angle = Random::RandomFloat(-45.0f, 45.0f) * PI_OVER_180;
	float angleDelta = (TERRAIN_CHAOSITY_BASE_ANGLE + (TERRAIN_CHAOSITY_ADDITIONAL_ANGLE * chaosity)) * PI_OVER_180;
	int minSeparation = fastround(TERRAIN_PLATFORMARITY_BASE + platformarity * TERRAIN_PLATFORMARITY_FACTOR);
	int angleSteps = 0;
	int lowSteps = 0;
	int lx = -1, ly = -1;
	
	bool down = false;
	bool back = false;

	// Allocate an array to hold minimum x distances across the buffer
	int* minX = new int[textureBuffer->Height()];
	
	for (int i = 0; i < textureBuffer->Height(); ++ i)
		minX[i] = -10000;

	for (int i = 1; i < numPoints; ++ i)
	{

		float d = Random::RandomFloat(minDistance, maxDistance);	

		p.x += cos(angle) * d;
		p.y += sin(angle) * d;

		// Clamp to right side
		if (p.x >= textureBuffer->Width())
		{

			i = numPoints;

		}

		// Make sure we aren't done
		if (i != numPoints)
		{

			// Set flags
			if (p.x < points[i - 1].x)
			{

				if (!back)
				{

					back = true;
			
					if (p.y > points[i - 1].y)
						down = true;
					else
						down = false;

				}

			}
			else
			{

				back = false;

			}

			int pix = (int)p.x;
			int piy = (int)p.y;

			if ((piy >= 0) && (piy < textureBuffer->Height()))
			{

				if (pix < minX[piy] + minSeparation)
					p.x = (float)(minX[piy] + minSeparation);

			}
			else
			{

				// Off the bottom, use zero index
				if (pix < minX[0] + minSeparation)
					p.x = (float)(minX[0] + minSeparation);

			}

			// Update with new min xs
			Vector2 v = p - points[i - 1];
			float distance = VectorLength(v);
			float lerp = 0.0f;
			VectorNormalize(&v);
			Vector2 c = points[i - 1];
			int cix, ciy;

			while (lerp < distance)
			{

				cix = (int)c.x;
				ciy = (int)c.y;

				if ((ciy >= 0) && (ciy < textureBuffer->Height()))
				{

					if (cix > minX[ciy])
						minX[ciy] = cix;

				}

				c.x += v.x;
				c.y += v.y;
				lerp += 1.0f;

			}

			if (back)
			{
				
				// Determine next angle

				angleSteps ++;

				if (angleSteps > TERRAIN_MAX_BACK_STEPS)
				{
				
					if (angle < 0)
						angle += Random::RandomFloat(0, angleDelta);
					else
						angle -= Random::RandomFloat(0, angleDelta);
				
				}
				else
					angle += Random::RandomFloat(-angleDelta, angleDelta);

			}
			else
			{

				// Moving right
				angleSteps = 0;

				// Prevent terrain height from getting too high/low
				if (p.y > textureBuffer->Height() * TERRAIN_MAX_HEIGHT)
				{

					angle -= Random::RandomFloat(0.0f, angleDelta);
					lowSteps = 0;

				}
				else if (p.y < textureBuffer->Height() * TERRAIN_MIN_HEIGHT)
				{

						
					// Allow a certain amount of steps below the minimum threshold before we start to move up
					if (lowSteps >= 10)
						angle += Random::RandomFloat(0.0f, angleDelta);
					else
						angle += Random::RandomFloat(-angleDelta, angleDelta);
						
					lowSteps ++;

				}
				else
				{

					// Adjust the terrain angle randomly
					angle += Random::RandomFloat(-angleDelta, angleDelta);
					lowSteps = 0;

				}

			}

			// Hard limit on angles
			const float angleLimitMax = 170.0f * PI_OVER_180;
			const float angleLimitMin = -170.0f * PI_OVER_180;
			if (angle > angleLimitMax)
				angle = angleLimitMax;
			else if (angle < angleLimitMin)
				angle = angleLimitMin;

			// Store the new point
			points.insert(points.end(), p);

		}
		else
		{

			// We reached the rhs, store the point and stop
			points.insert(points.end(), p);
			break;

		}

	}

	// Process the points to form the terrain image
	ProcessPoints();

	// Add caverns
	int numCaverns = fastround(cavernacity * TERRAIN_MAX_CAVERNACITY);
	for (int i = 0; i < numCaverns; ++ i)
		CreateCavern(Random::RandomInt(0, textureBuffer->Width()), Random::RandomInt(0, textureBuffer->Height()), smoothness, chaosity, 0);

#ifdef PB_DEBUG
	textureBuffer->SaveAsRAW("generated.raw");
#endif

	// Buffer all textures
	BufferAll();

	return true;

}

//
// Rendering
//

void Terrain::Render()
{

	if (!IsValid())
		return;

	Renderer* renderer = Renderer::Get();

	// Draw all terrain blocks
	int index;
	for (int i = 0; i < height; ++ i)
	{

		for (int j = 0; j < width; ++ j)
		{

			index = j + i * width;

			if ((blocks[index]) && (!blocks[index]->IsEmpty()))
				renderer->Render(blocks[index]->Sprite());

		}

	}


}

//
// Accessors
//

int Terrain::Seed()
{

	return genSeed;

}

int Terrain::Width()
{

	return width;

}

int Terrain::Height()
{

	return height;

}

int Terrain::NumBlocks()
{

	return numBlocks;

}

int Terrain::WidthInPixels()
{

	return textureBuffer->Width();

}

int Terrain::HeightInPixels()
{

	return textureBuffer->Height();

}



