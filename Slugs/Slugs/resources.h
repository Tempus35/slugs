#pragma once

#include <sfml/graphics.hpp>
#include <sfml/audio.hpp>

/*
	class Resource
	Base class for resources
*/

class Resource
{

public:

	//
	// Initialization
	//

	Resource();
	virtual ~Resource();

};

/*
	class ImageResource
	Container for SFML Sound
*/

class SoundResource : public Resource
{

private:
	
	sf::SoundBuffer Buffer;

public:

	SoundResource(char* path);
	 ~SoundResource();

	sf::SoundBuffer& Sound();

};

/*
	class ImageResource
	Container for SFML Image
*/

class ImageResource : public Resource
{

private:

	sf::Image image;

public:

	//
	// Initialization
	//

	ImageResource(int width, int height, bool smooth);
	ImageResource(char* path);
	~ImageResource();

	//
	// Accessors
	//

	sf::Image& Image();

};