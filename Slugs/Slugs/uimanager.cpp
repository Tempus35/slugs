#include "uimanager.h"

UIManager::UIManager()
{

	inputHook = NULL;

}
	
UIManager::~UIManager()
{

	RemoveAllWidgets();

}

bool UIManager::Exists(UIWidget* widget)
{

	std::list<UIWidget*>::iterator i = widgets.begin();

	while (i != widgets.end())
	{

		if (*i == widget)
			return true;

		i ++;

	}

	return false;

}

void UIManager::AddWidget(UIWidget* widget)
{

	if (!Exists(widget))
	{

		// Set self as the widget owner
		widget->SetOwner(this);

		// Add the widget
		widgets.insert(widgets.end(), widget);

	}

}

void UIManager::RemoveWidget(UIWidget* widget)
{

	widgets.remove(widget);

}

void UIManager::RemoveWidgetsInGroup(unsigned int group)
{

	if (group == UIGroup_All)
	{

		RemoveAllWidgets();

	}
	else if (group != UIGroup_None)
	{

		std::list<UIWidget*>::iterator i = widgets.begin();

		while (i != widgets.end())
		{

			UIWidget* w = (*i);

			if (w->GetGroup() & group)
			{

				SafeDelete(w);
				i = widgets.erase(i);

			}
			else
				i ++;

		}

	}

}

void UIManager::RemoveAllWidgets()
{

	std::list<UIWidget*>::iterator i = widgets.begin();

	while (i != widgets.end())
	{

		SafeDelete(*i);
		i ++;

	}

	widgets.clear();

}

void UIManager::Update(float elapsedTime)
{

	std::list<UIWidget*>::iterator i = widgets.begin();

	while (i != widgets.end())
	{

		(*i)->Update(elapsedTime);

		i++;

	}

}

void UIManager::Render()
{

	std::list<UIWidget*>::iterator i = widgets.begin();

	while (i != widgets.end())
	{

		UIWidget* w = (*i);

		if (w->IsVisible())
			w->Render();

		i++;

	}

}

UIWidget* UIManager::GetWidget(const std::string& name)
{

	std::list<UIWidget*>::iterator i = widgets.begin();

	while (i != widgets.end())
	{

		UIWidget* w = (*i);

		if (w->GetName() == name)
			return w;

		i ++;

	}

	return NULL;

}

bool UIManager::KeyDown(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (inputHook)
		return inputHook->KeyDown(key, shift, control, alt);

	return false;

}

bool UIManager::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if (inputHook)
		return inputHook->KeyUp(key, shift, control, alt);

	return false;

}

void UIManager::SetInputHook(UIWidget* widget)
{

	inputHook = widget;

}