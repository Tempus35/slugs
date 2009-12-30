#include "uiloadgraphic.h"

UILoadGraphic::UILoadGraphic(ImageResource* imageResource, const std::string& _name, unsigned int _group, bool _visible, bool _enabled) : UIWidget(_name, _group, _visible, _enabled)
{

	sprite.SetImage(imageResource);
	sprite.Center();

}

void UILoadGraphic::Update(float elapsedTime)
{

	const float FILL_RATE = Radians(180.0f);	

	pie.AdjustFill(FILL_RATE * elapsedTime);

}

void UILoadGraphic::Render()
{

	Renderer::Get()->Render(sprite);
	Renderer::Get()->Render(pie);

}

void UILoadGraphic::SetPosition(const Vec2i& position)
{

	Vec2f p = ConvertVector(position);

	sprite.SetPosition(p);
	pie.SetPosition(p);

	UIWidget::SetPosition(position);

}