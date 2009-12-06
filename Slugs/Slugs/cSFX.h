#pragma once

#include "soundresource.h"

class cSFX
{
public:
	cSFX();
	~cSFX();

	bool bLoadSnd(SoundResource *res);

	void PlaySnd();
	void PlaySnd(bool bLoop);
	void Stop();

	//Volume
	void change_volume(float Volume);
	void set_volume(float Volume);
	float get_volume();

private:
	sf::SoundBuffer Buffer;
	sf::Sound Sound;
	float fVolume;
};
