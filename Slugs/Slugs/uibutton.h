#pragma once

#include "uiwidget.h"

enum UIButtonState
{

	UIButtonState_Up		= 0,
	UIButtonState_Down		= 1,
	UIButtonState_Hover		= 2,

};

class UIButton : public UIWidget
{

protected:

	ImageResource*		imageResource[3];
	UIButtonState		state;
	int					clickEvent;

public:

};