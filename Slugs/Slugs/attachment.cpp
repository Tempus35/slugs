#include "attachment.h"

/*
	class Attachment
*/

// Constructor
Attachment::Attachment(ImageResource* imageResource)
{

	ASSERT(imageResource);
	sprite.SetImage(imageResource);
	sprite.Center();

}

// Renders the attachment
void Attachment::Render(const Vec2f& position)
{

	sprite.SetPosition(position.x, position.y);
	Renderer::Get()->Render(sprite);

}

void Attachment::SetMirrored(bool state)
{

	sprite.SetMirrored(state);

}