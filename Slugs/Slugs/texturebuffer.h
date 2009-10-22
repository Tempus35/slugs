#pragma once

#include <fstream>
#include <iostream>

#include "defs.h"
#include "resources.h"

using namespace std;

/*
	class TextureBuffer
	Memory storage for rgb(a)/single channel texture data which does not require rendering
*/

class TextureBuffer : public Resource
{

private:
	int width, height;
	int channels;
	byte* data;

public:

	int dataSize;

	//
	// Initialization
	//
	TextureBuffer(char* path, int textureWidth, int textureHeight, int numChannels);
	TextureBuffer(int textureWidth, int textureHeight, int numChannels);
	~TextureBuffer();
	bool IsValid();

	//
	// Modification
	//

	void Fill(byte b);

	//
	// Output
	//

	bool SaveAsRAW(char* path);

	//
	// Helpers
	//

	bool Intersection(int x0, int y0, int x1, int y1, int* x, int* y);
	inline int PositionToIndex(int x, int y) { return ((x < 0) || (x >= width) || (y < 0) || (y >= height)) ? -1 : x + y * width; };

	//
	// Accessors
	//

	int Width();
	int Height();
	int Channels();
	byte* Data(int index = 0);
	byte* Data(int x, int y);

};