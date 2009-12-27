#pragma once

#include "box.h"
#include "imageresource.h"

class UIWidget
{

protected:

	Boxi			bounds;			// Screen space bounding box of the widget

public:

	// Gets the bounds of the widget
	const Boxi& GetBounds() const;

	// Sets the position of the widget
	void SetPosition(const Vec2i& position);

	// Returns true if the widget contains a point
	bool Contains(const Vec2i& point);

};