//---------------------------------------------------------------
//
// Slugs
// camera.cpp
//
//---------------------------------------------------------------

#include "camera.h"
#include "world.h"

Camera::Camera()
{

}

void Camera::SetPosition(const Vec2f& point)
{

	view.SetCenter(point.x, -point.y);

}

void Camera::SetPosition(int x, int y)
{

	view.SetCenter((float)x, -(float)y);

}

void Camera::SetViewSize(int width, int height)
{

	view.SetFromRect(sf::FloatRect(0, 0, (float)width, (float)height));

}

const sf::View& Camera::GetView() const
{

	return view;

}

Vec2f Camera::GetPosition() const
{

	return Vec2f(view.GetCenter());

}

Vec2f Camera::GetWorldPosition() const
{

	const sf::FloatRect& rect = view.GetRect();

	return GetWorldPosition((int)(rect.GetWidth() * 0.5f), (int)(rect.GetHeight() * 0.5f));

}

Vec2f Camera::GetWorldPosition(int screenX, int screenY) const
{

	const sf::FloatRect& rect = view.GetRect();

	return Vec2f(rect.Left + screenX, -rect.Bottom + (rect.GetHeight() - screenY));

}

void Camera::Move(int deltaX, int deltaY)
{

	view.Move((float)deltaX, (float)deltaY);

}

void Camera::MoveTo(Object* object)
{

	// TODO : Move over time
	view.Move(object->GetPosition().x, object->GetPosition().y);

}
