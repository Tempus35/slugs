#pragma once

#include <sfml/audio.hpp>

#include "resource.h"

/*
	class SoundResource
	Container for SFML Sound
*/

class SoundResource : public Resource
{

private:
	
	sf::SoundBuffer Buffer;

public:

	SoundResource(const std::string& path);
	 ~SoundResource();

	sf::SoundBuffer& Sound();

};