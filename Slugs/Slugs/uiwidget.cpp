#include "uiwidget.h"

UIWidget::UIWidget(const std::string& _name, unsigned int _group, bool _visible, bool _enabled)
{

	owner = NULL;

	name = _name;

	group = _group;

	visible = _visible;
	enabled = _enabled;

}

UIWidget::UIWidget(const std::string& _name, unsigned int _group, const Vec2i& position, const Vec2i& extents)
{

	owner = NULL;

	name = _name;

	group = _group;

	bounds = Boxi(position, extents);

	visible = true;
	enabled = true;

}

void UIWidget::SetOwner(UIManager* _owner)
{

	owner = _owner;

}

const std::string& UIWidget::GetName() const
{

	return name;

}

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

void UIWidget::Update(float elapsedTime)
{

}

void UIWidget::Render()
{

}

bool UIWidget::IsEnabled() const
{

	return enabled;

}

void UIWidget::SetEnabled(bool state)
{

	enabled = state;

}

bool UIWidget::IsVisible() const
{

	return visible;

}

void UIWidget::SetVisible(bool state)
{

	visible = state;

}

void UIWidget::SetGroup(unsigned int newGroup)
{

	group = newGroup;

}

unsigned int UIWidget::GetGroup() const
{

	return group;

}

bool UIWidget::KeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	return false;

}

bool UIWidget::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	return false;

}
