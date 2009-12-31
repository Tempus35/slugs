#pragma once

#include <sfml/graphics.hpp>
#include <sfml/system.hpp>

#include "global.h"
#include "renderer.h"
#include "vec2.h"
#include "box.h"
#include "imageresource.h"
#include "fontresource.h"
#include "sprite.h"
#include "shape.h"
#include "converters.h"
#include "localizer.h"

/*
	Forward declarations
*/

class UIManager;

/*
	Enumeration of UI groups
*/

enum UIGroup
{

	UIGroup_All				= -1,
	UIGroup_None			= 0,
	UIGroup_System			= 1,		// Never cleared (console)
	UIGroup_Game			= 2,		// Anything used in the main game
	UIGroup_Other			= 4,		// Temporary store for stuff like the loading graphic that will be cleared often

};

/*
	class UIWidget
	Base class for all ui widgets
*/

class UIWidget
{

protected:

	UIManager*		owner;			// Pointer to owning UIManager instance
	std::string		name;			// Name of the widget

	Boxi			bounds;			// Screen space bounding box of the widget
	bool			visible;		// Is the widget visible
	bool			enabled;		// Is the widget enabled (does it respond to user input?)

	unsigned int	group;			// Group to which this widget belongs

public:

	// Constructors
	UIWidget(const std::string& _name, unsigned int _group = UIGroup_None, bool _visible = true, bool _enabled = true);
	UIWidget(const std::string& _name, unsigned int _group, const Vec2i& position, const Vec2i& extents);

	// Sets the owner of the widget
	virtual void SetOwner(UIManager* _owner);

	// Gets the name of the widget
	virtual const std::string& GetName() const;

	// Gets the bounds of the widget
	virtual const Boxi& GetBounds() const;

	// Sets the position of the widget
	virtual void SetPosition(const Vec2i& position);

	// Returns true if the widget contains a point
	virtual bool Contains(const Vec2i& point);

	// Updates the widget
	virtual void Update(float elapsedTime);

	// Renders the widget
	virtual void Render();

	// Returns true if the widget is visible
	virtual bool IsVisible() const;

	// Sets the visibility of the widget
	virtual void SetVisible(bool state);

	// Returns true if the widget is enabled
	virtual bool IsEnabled() const;

	// Sets if the widget is enabled
	virtual void SetEnabled(bool state);

	// Sets the group the widgets belongs to
	virtual void SetGroup(unsigned int newGroup);

	// Gets the group the widgets belongs to
	virtual unsigned int GetGroup() const;

	// Handles a key press
	virtual bool KeyDown(sf::Key::Code key, bool shift, bool control, bool alt);

	// Handles a key release
	virtual bool KeyUp(sf::Key::Code key, bool shift, bool control, bool alt);

};