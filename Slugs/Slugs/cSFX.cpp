#include "cSFX.h"

cSFX::cSFX() {
	fVolume = 25.0f;
}

cSFX::~cSFX() {
}

bool cSFX::bLoadSnd(SoundResource *res) {

	if (res == NULL) 
		return false;

	Buffer = res->Sound();

	Sound.SetBuffer(Buffer); 
	Sound.SetLoop(false);
	Sound.SetPitch(1.5f);
	Sound.SetVolume(fVolume);


	return true;
}
	
void cSFX::PlaySnd() {
	Sound.Play();
}

void cSFX::PlaySnd(bool bLoop) {
	Sound.SetLoop(bLoop);
	Sound.Play();
}

void cSFX::change_volume(float Volume) {
	fVolume += Volume;
	Sound.SetVolume(fVolume);
}

void cSFX::set_volume(float Volume) {
	fVolume = Volume;
	Sound.SetVolume(fVolume);
}

float cSFX::get_volume() {
	return fVolume;
}

void cSFX::Stop() {
	Sound.Stop();
}