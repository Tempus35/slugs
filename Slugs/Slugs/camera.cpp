//---------------------------------------------------------------
//
// Slugs
// camera.cpp
//
//---------------------------------------------------------------

#include "camera.h"
#include "game.h"

Camera::Camera()
{

	action = CameraAction_None;

}

void Camera::SetPosition(const Vec2f& point)
{

	position = point;
	view.SetCenter(point.x, -point.y);

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

	return position;

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

void Camera::Move(int deltaX, int deltaY, bool internalMove)
{

	float x = (float)deltaX;
	float y = (float)deltaY;

	position += Vec2f(x, y);
	view.Move(x, -y);

	if (!internalMove)
		action = CameraAction_None;

}

void Camera::MoveTo(Object* object, bool instant)
{

	if (instant)
	{

		position = object->GetPosition();
		action = CameraAction_None;

		view.SetCenter(position.x, -position.y);

	}
	else
	{

		Vec2f clampedTarget;
		const sf::FloatRect& rect = view.GetRect();

		float halfWidth = rect.GetWidth() / 2.0f;
		float halfHeight = rect.GetHeight() / 2.0f;

		clampedTarget.x = Clamp(object->GetPosition().x, halfWidth, Game::Get()->GetWorld()->WidthInPixels() - halfWidth);
		clampedTarget.y = Clamp(object->GetPosition().y, halfHeight, Game::Get()->GetWorld()->HeightInPixels() - halfHeight);

		target = clampedTarget;

		action = CameraAction_Move;

	}

}

bool Camera::Update(float elapsedTime)
{

	const float CAMERA_MOVE_SPEED = 1000.0f;		// Movement speed of camera in pixels/second

	switch (action)
	{

	case CameraAction_Move:

		Vec2f direction = target - position;
		float distanceToTarget = direction.Length();

		if (distanceToTarget >= 1.0f)
		{

			float distanceToMove = Min(distanceToTarget, elapsedTime * CAMERA_MOVE_SPEED);

			direction = direction.Normalize() * distanceToMove;
			Move(RoundDownToInt(direction.x), RoundDownToInt(direction.y), true);

			return true;

		}
		else
			action = CameraAction_None;

		break;

	}

	return false;

}
