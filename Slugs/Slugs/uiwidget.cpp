#include "uiwidget.h"

const Boxi& UIWidget::GetBounds() const
{

	return bounds;

}

void UIWidget::SetPosition(const Vec2i& position)
{

	bounds.center = position;

}

bool UIWidget::Contains(const Vec2i& point)
{

	return bounds.Contains(point);

}