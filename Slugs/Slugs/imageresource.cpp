#include "imageresource.h"

/*
	class ImageResource
*/

ImageResource::ImageResource(int width, int height, bool smooth)
{

	image.Create(width, height);
	image.SetSmooth(smooth);

}

ImageResource::ImageResource(const std::string& path)
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