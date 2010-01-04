#include "bufferresource.h"

/*
	class BufferResource
*/

BufferResource::BufferResource(const std::string& path) : Resource(ResourceType_Buffer)
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

BufferResource::BufferResource(int textureWidth, int textureHeight, int numChannels) : Resource(ResourceType_Buffer)
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

BufferResource::~BufferResource()
{

	// Free memory
	if (data)
		delete[] data;

}

bool BufferResource::IsValid() const
{

	return (data != NULL);

}

void BufferResource::Fill(byte b)
{

	if (data)
		memset(data, b, dataSize);

}

bool BufferResource::SaveAsRAW(const std::string& path) const
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

bool BufferResource::Intersection(int x0, int y0, int x1, int y1, int* x, int* y) const
{

	// Clamp to buffer size
	x[0] = Max(0, x0);
	y[0] = Max(0, y0);
	x[1] = Min(x1, width);
	y[1] = Min(y1, height);

	if ((x[1] != x[0]) || (y[1] != y[0]))
		return true;

	return false;

}

int BufferResource::GetWidth() const
{

	return width;

}

int BufferResource::GetHeight() const
{

	return height;

}

int BufferResource::GetChannels() const
{

	return channels;

}

byte* BufferResource::GetData(int index) const
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

byte* BufferResource::GetData(int x, int y) const
{

	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return NULL;
	else
		return &data[x * channels + y * width * channels];

}

void BufferResource::BurnIn(BufferResource* buffer, int x, int y)
{

	Vec2i destStart = Vec2i(Max(x, 0), Max(y, 0));
	Vec2i destEnd = Vec2i(Min(x + buffer->GetWidth(), width - 1), Min(y + buffer->GetHeight(), height - 1));

	Vec2i srcStart = Vec2i(destStart.x - x, destStart.y - y);
	
	for (int y = destStart.y; y < destEnd.y; ++ y)
	{

		for (int x = destStart.x; x < destEnd.x; ++ x)
		{

			byte* ptr = buffer->GetData(srcStart.x + x - destStart.x, srcStart.y + y - destStart.y);

			if (ptr[3] > 0x00)
			{

				int offset = x * channels + y * width * channels;
				data[offset] = *ptr;
				data[offset + 1] = *(ptr + 1);
				data[offset + 2] = *(ptr + 2);
				data[offset + 3] = *(ptr + 3);

			}

		}

	}

}

int BufferResource::GetDataSize() const
{

	return dataSize;

}