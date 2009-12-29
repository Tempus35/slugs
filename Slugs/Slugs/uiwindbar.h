#pragma once

#include "uiwidget.h"

enum UIWindBarImage
{

	UIWindBarImage_Unfilled = 0,
	UiWindBarImage_Filled = 1,

};

class UIWindBar : public UIWidget
{

protected:

	ImageResource*		imageResource[2];

	Sprite				sprite;

public:

};