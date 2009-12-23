#include "soundresource.h"

/*
	class SoundResource
*/

SoundResource::SoundResource(const std::string& path)
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