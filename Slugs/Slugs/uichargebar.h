#pragma once

#include "uiwidget.h"

/*
	class UIChargebar
	Weapon charging indicator
*/

class UIChargebar : public UIWidget
{

protected:

	Sprite			background;
	Sprite			fill;

public:

	UIChargebar(const std::string& name, int group, ImageResource* backgroundResource, ImageResource* fillResource);

	virtual void Render();

	virtual void SetPosition(const Vec2i& position);

};