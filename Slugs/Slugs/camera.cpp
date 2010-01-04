//---------------------------------------------------------------
//
// Slugs
// camera.cpp
//
//---------------------------------------------------------------

#include "camera.h"
#include "game.h"
#include "object.h"

Camera::Camera()
{

	action = CameraAction_None;
	targetObject = NULL;

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

	if (Game::Get()->GetGameBool(GameBool_LockCameraToLevel))
	{

		const sf::FloatRect& rect = view.GetRect();

		if (rect.Left + x < 0.0f)
			x = 0.0f;

		if (-rect.Bottom + y < 0.0f)
			y = 0.0f;

		if (rect.Right + x > Game::Get()->GetWorld()->WidthInPixels())
			x = 0.0f;

		if (-rect.Top + y > Game::Get()->GetWorld()->HeightInPixels())
			y = 0.0f;

	}

	position += Vec2f(x, y);
	view.Move(x, -y);

	if (!internalMove)
		action = CameraAction_None;

}

void Camera::MoveTo(Object* object, bool track, bool instant)
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

	// Set the object as the target if we are going to track it
	if (track)
		targetObject = object;

}

bool Camera::Update(float elapsedTime)
{

	// If our target object is dead, stop tracking it
	if ((targetObject) && (!targetObject->IsAlive()))
		StopTracking();

	const float CAMERA_MOVE_SPEED = 1000.0f;		// Movement speed of camera in pixels/second

	// Continue tracking if we have a target object
	if ((targetObject) && (action == CameraAction_None))
		action = CameraAction_Track;

	if (action == CameraAction_Move)
	{

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

	}
	else if (action == CameraAction_Track)
	{

		Vec2f direction = targetObject->GetPosition() - position;
		float distanceToTarget = direction.Length();

		if (distanceToTarget >= 1.0f)
		{

			float distanceToMove = Min(distanceToTarget, elapsedTime * targetObject->GetSpeed());

			direction = direction.Normalize() * distanceToMove;
			Move(RoundDownToInt(direction.x), RoundDownToInt(direction.y), true);

			return true;

		}
		else
			action = CameraAction_None;

	}

	return false;

}

void Camera::StartTracking(Object* object)
{

	if (Game::Get()->GetGameState() == GameState_Game)
	{

		ASSERTMSG(object != NULL, "Can't track NULL object. Did you mean to use StopTracking()?");
		targetObject = object;
		action = CameraAction_Track;

	}

}

void Camera::StopTracking()
{

	targetObject = NULL;
	action = CameraAction_None;

}
