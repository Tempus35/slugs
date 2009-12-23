#include "texturebuffer.h"

/*
	class TextureBuffer
*/

//
// Initialization
//

TextureBuffer::TextureBuffer(char* path, int textureWidth, int textureHeight, int numChannels)
{

	data = NULL;

	if ((path) && (textureWidth > 0) && (textureHeight > 0) && ((numChannels == 1) || (numChannels == 3) || (numChannels == 4)))
	{

		// Open file
		ifstream fs(path, ios::in|ios::binary);

		if (fs.is_open())
		{

			// Store parameters
			width = textureWidth;
			height = textureHeight;
			channels = numChannels;

			// Allocate memory
			dataSize = width * height * channels;
			data = new byte[dataSize];

			// Get data
			if (data)
				fs.read((char*)data, dataSize);

			// Close file
			fs.close();

		}

	}

}

TextureBuffer::TextureBuffer(int textureWidth, int textureHeight, int numChannels)
{

	data = NULL;

	if ((textureWidth > 0) && (textureHeight > 0) && (numChannels > 0))
	{

		// Store params
		width = textureWidth;
		height = textureHeight;
		channels = numChannels;

		// Allocate memory
		dataSize = width * height * channels;
		data = new byte[dataSize];

		// Fill with zeros
		memset(data, 0, dataSize);

	}

}

TextureBuffer::~TextureBuffer()
{

	// Free memory
	if (data)
		delete[] data;

}

bool TextureBuffer::IsValid()
{

	return (data != NULL);

}

//
// Modification
//

void TextureBuffer::Fill(byte b)
{

	if (data)
		memset(data, b, dataSize);

}

//
// Output
//

bool TextureBuffer::SaveAsRAW(char* path)
{

	if ((path) && (data))
	{

		// Open file
		ofstream fs(path, ios::out|ios::trunc|ios::binary);

		if (fs.is_open())
		{

			// Write data
			fs.write((char*)data, dataSize);

			// Close file
			fs.close();

			return true;

		}

	}

	return false;

}

//
// Helpers
//

bool TextureBuffer::Intersection(int x0, int y0, int x1, int y1, int* x, int* y)
{

	// Clamp to buffer size
	x[0] = max(0, x0);
	y[0] = max(0, y0);
	x[1] = min(x1, width - 1);
	y[1] = min(y1, height - 1);

	if ((x[1] != x[0]) || (y[1] != y[0]))
		return true;

	return false;

}

//
// Accessors
//

int TextureBuffer::Width()
{

	return width;

}

int TextureBuffer::Height()
{

	return height;

}

int TextureBuffer::Channels()
{

	return channels;

}

byte* TextureBuffer::Data(int index)
{

	if (index == 0)
		return data;
	else
	{

		if ((index < 0) || (index >= dataSize))
			return NULL;
		else
			return &data[index];

	}

}

byte* TextureBuffer::Data(int x, int y)
{

	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return NULL;
	else
		return &data[x * channels + y * width * channels];

}
