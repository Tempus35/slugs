//---------------------------------------------------------------
//
// Slugs
// camera.h
//
//---------------------------------------------------------------

#pragma once

#include <sfml/graphics.hpp>

#include "vec2.h"
#include "object.h"

/*
	class Camera
*/

class Camera
{

private:

	sf::View view;

public:

	// Constructor
	Camera();

	// Moves the camera so that is focuses on a point in world space
	void SetPosition(const Vec2f& point);
	void SetPosition(int x, int y);

	// Sets the view size (should be equal to window size)
	void SetViewSize(int width, int height);

	// Gets the associated view
	const sf::View& GetView() const;

	// Gets the current camera position
	Vec2f GetPosition() const;

	// Gets the world space position on which the camera is focused
	Vec2f GetWorldPosition() const;

	// Gets the world space position associated with a position in screen space
	Vec2f GetWorldPosition(int screenX, int screenY) const;

	// Moves the camera
	void Move(int deltaX, int deltaY);

	// Moves the camera to an object
	void MoveTo(Object* object);

};