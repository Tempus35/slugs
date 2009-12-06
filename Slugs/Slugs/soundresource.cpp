#include "soundresource.h"

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