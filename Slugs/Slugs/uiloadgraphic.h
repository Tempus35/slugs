#pragma once

#include "uiwidget.h"

/*
	class UILoadGraphic
*/

class UILoadGraphic : public UIWidget
{

protected:

	Sprite			sprite;
	Shape_Pie		pie;

public:

	UILoadGraphic(ImageResource* imageResource, const std::string& _name, unsigned int _group = UIGroup_None, bool _visible = true, bool _enabled = true);

	virtual void Update(float elapsedTime);

	virtual void Render();
	
	virtual void SetPosition(const Vec2i& position);

};