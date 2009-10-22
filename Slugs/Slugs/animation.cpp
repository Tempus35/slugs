#include "animation.h"

Animation::Animation(ImageResource* res, int framesWide, int framesHigh, Color color, float framesPerSecond) : Sprite(res, color)
{

	sf::Vector2f size = data.GetSize();

	frames[0] = framesWide;
	frames[1] = framesHigh;

	frameWidth = (int)(size.x / (float)frames[0]);
	frameHeight = (int)(size.y / (float)frames[1]);
	
	numFrames = frames[0] * frames[1];
	currentFrame = 0;

	SetFPS(framesPerSecond);

	SetFrameRect(0);

}

Animation::~Animation()
{

}

//
// Private methods
//

void Animation::SetFrameRect(int frame)
{

	currentFrame = frame;

	int column = frame % frames[0];
	int row = frame / frames[0];
	
	r.Left		= column * frameWidth;
	r.Top		= row * frameHeight;
	r.Right		= r.Left + frameWidth;
	r.Bottom	= r.Top + frameHeight;

	data.SetSubRect(r);

}

//
// Public methods
//

void Animation::Update(float elapsedTime)
{

	frameTimer -= elapsedTime;

	while (frameTimer <= 0.0f)
	{

		frameTimer += frameTime;
		
		currentFrame ++;

		if (currentFrame == numFrames)
			currentFrame = 0;

	}

}

//
// Accessors
//

void Animation::SetFrame(int frame)
{

	int newFrame = frame % numFrames;

	if (newFrame != currentFrame)
		SetFrameRect(newFrame);

}

void Animation::SetFPS(float fps)
{

	frameTime = 1.0f / fps;
	frameTimer = frameTime;

}