#pragma once

#include <list>

#include "singleton.h"
#include "uiwidget.h"

class UIManager : public Singleton<UIManager>
{

friend class Singleton<UIManager>;

private:

	std::list<UIWidget*> widgets;

public:

};