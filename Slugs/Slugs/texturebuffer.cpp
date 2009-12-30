#include "texturebuffer.h"

/*
	class TextureBuffer
*/

TextureBuffer::TextureBuffer(const std::string& path) : Resource(ResourceType_TextureBuffer)
{

	ilInit();

	unsigned int image;
	ilGenImages(1, &image);
	ilBindImage(image);

	ilLoadImage(path.c_str());

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT); 
	channels = 4;

	int dataSize = width * height * channels;
	data = new byte[dataSize];
	
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);

	ilDeleteImages(1, &image);

}

TextureBuffer::TextureBuffer(int textureWidth, int textureHeight, int numChannels) : Resource(ResourceType_TextureBuffer)
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

void TextureBuffer::Fill(byte b)
{

	if (data)
		memset(data, b, dataSize);

}

bool TextureBuffer::SaveAsRAW(const std::string& path)
{

	if ((!path.empty()) && (data))
	{

		// Open file
		std::ofstream fs(path.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);

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

bool TextureBuffer::Intersection(int x0, int y0, int x1, int y1, int* x, int* y)
{

	// Clamp to buffer size
	x[0] = Max(0, x0);
	y[0] = Max(0, y0);
	x[1] = Min(x1, width - 1);
	y[1] = Min(y1, height - 1);

	if ((x[1] != x[0]) || (y[1] != y[0]))
		return true;

	return false;

}

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
