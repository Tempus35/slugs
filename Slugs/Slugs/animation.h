#pragma once

#include "resources.h"
#include "sprite.h"

#define ANIMATION_DEFAULT_FPS 10

class Animation : public Sprite
{

private:

	int frameWidth;						// Width of a single frame in pixels
	int frameHeight;					// Height of a single frame in pixels
	int frames[2];						// Frame grid size

	int numFrames;						// Number of frames in animation
	int currentFrame;					// Current frame

	float fps;							// Frames per second of animation
	float frameTime, frameTimer;		

	sf::IntRect r;

	//
	// Private methods
	//

	void SetFrameRect(int frame);

public:

	Animation(ImageResource* res, int framesWide, int framesHigh, Color color = Color(255, 255, 255), float framesPerSecond = ANIMATION_DEFAULT_FPS);
	~Animation();

	//
	// Public methods
	//

	void Update(float elapsedTime);

	//
	// Accessors
	//

	void SetFrame(int frame);
	void SetFPS(float fps);

};