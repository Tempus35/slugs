#pragma once

#include <list>

#include "global.h"
#include "uiwidget.h"
#include "uiconsole.h"
#include "uiloadgraphic.h"
#include "uiwindbar.h"
#include "uiteambar.h"
#include "uitimer.h"

class UIManager
{

private:

	std::list<UIWidget*>			widgets;						// List of all widgets currently on the UI
	UIWidget*						inputHook;						// Pointer to a widget taking exclusive control of input (if any)

private:

	// Returns true if the widget already exists on the UI
	bool Exists(UIWidget* widget);

public:

	// Constructor
	UIManager();
	
	// Destructor
	~UIManager();

	// Adds a widget to the UI
	void AddWidget(UIWidget* widget);

	// Removes a widget from the UI
	void RemoveWidget(UIWidget* widget);

	// Removes all widgets in the specified group
	void RemoveWidgetsInGroup(unsigned int group = UIGroup_All);

	// Removes all widgets from the UI
	void RemoveAllWidgets();

	// Updates the UI
	void Update(float elapsedTime);

	// Renders the UI
	void Render();

	// Gets a widget by name
	UIWidget* GetWidget(const std::string& name);

	// Processes a key press. Returns false if the event wasn't handled.
	bool KeyDown(sf::Key::Code key, bool shift, bool control, bool alt);

	// Processes a key release. Returns false if the event wasn't handled.
	bool KeyUp(sf::Key::Code key, bool shift, bool control, bool alt);

	// Sets the exclusive input hook
	void SetInputHook(UIWidget* widget);

};