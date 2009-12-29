#pragma once

#include "uiwidget.h"

enum UITeamBarImage
{

	UITeamBarImage_Empty = 0,
	UITeamBarImage_Full = 1,

};

class UITeamBar : public UIWidget
{

protected:

	ImageResource*		imageResource[2];

	Sprite				sprite;

public:

};