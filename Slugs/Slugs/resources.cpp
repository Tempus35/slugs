// +PB

#include "resources.h"

/*
	class Resource
*/

Resource::Resource()
{

}

Resource::~Resource()
{

}

/*
	class SoundResource
*/

SoundResource::SoundResource(char* path)
{

	Buffer.LoadFromFile(path);

}

SoundResource::~SoundResource()
{

}

sf::SoundBuffer& SoundResource::Sound()
{

	return Buffer;

}


/*
	class ImageResource
*/

ImageResource::ImageResource(int width, int height, bool smooth)
{

	image.Create(width, height);
	image.SetSmooth(smooth);

}

ImageResource::ImageResource(char* path)
{

	image.LoadFromFile(path);

}

ImageResource::~ImageResource()
{

}

sf::Image& ImageResource::Image()
{

	return image;

}