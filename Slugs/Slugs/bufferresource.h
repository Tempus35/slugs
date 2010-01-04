#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "il.h"

#include "global.h"
#include "resource.h"

/*
	class BufferResource
	Resource manager storage container for arbitrary data
*/

class BufferResource : public Resource
{

private:

	int			width;			// Width of the buffer in pixels		
	int			height;			// Height of the buffer in pixels
	int			channels;		// Numbers of channels
	byte*		data;			// Pointer to data
	int			dataSize;		// Size of the data in bytes (width * height * channels)

public:

	// Constructors
	BufferResource(const std::string& path);
	BufferResource(int textureWidth, int textureHeight, int numChannels);

	// Destructor
	~BufferResource();

	// Returns true if this image is valid (image data filled out correctly)
	bool IsValid() const;

	// Clears the buffer to a byte value
	void Fill(byte b);

	// Saves the contents of the buffer as raw data
	bool SaveAsRAW(const std::string& path) const;

	// Calculates the bounding box that intersects the buffer
	bool Intersection(int x0, int y0, int x1, int y1, int* x, int* y) const;

	// Converts an x/y position to a buffer position
	inline int PositionToIndex(int x, int y) { return ((x < 0) || (x >= width) || (y < 0) || (y >= height)) ? -1 : x + y * width ; };

	// Returns the width of the buffer
	int GetWidth() const;

	// Returns the height of the buffer
	int GetHeight() const;
	
	// Returns the number of channels in the buffer
	int GetChannels() const;

	// Returns a pointer to the data at index
	byte* GetData(int index = 0) const;

	// Returns a pointer to the data for an x/y position
	byte* GetData(int x, int y) const;

	// Burns the contents of a texture buffer into this texture buffer
	void BurnIn(BufferResource* buffer, int x, int y);

	// Returns the size of the buffer in bytes
	int GetDataSize() const;

};