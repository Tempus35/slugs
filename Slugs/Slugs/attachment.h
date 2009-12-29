#pragma once

#include <vector>
#include <sfml/graphics.hpp>

#include "renderer.h"
#include "vec2.h"

class Attachment
{

protected:

	Sprite					sprite;

public:

	// Constructor
	Attachment(ImageResource* imageResource);

	// Renders the attachment
	virtual void Render(const Vec2f& position);

	// Sets if the attachment should be displayed mirrored
	void SetMirrored(bool state);

};

struct AttachPoint
{

	Vec2f						offset;			// Offset from the center of the object (when right facing)
	std::vector<Attachment*>	attachments;	// List of attachments attached to this point

};