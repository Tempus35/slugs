#pragma once

#include "game.h"
#include "uiwidget.h"

class UITimer : public UIWidget
{

protected:

	Sprite				background;
	FontResource*		fontResource;
	char				buffer[16];

public:

	UITimer(const std::string& name, int group, ImageResource* backgroundImage, FontResource* font);

	virtual void Render();

	virtual void SetPosition(const Vec2i& position);

};