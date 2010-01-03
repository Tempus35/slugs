//---------------------------------------------------------------
//
// Slugs
// object.cpp
//
//---------------------------------------------------------------

#include "object.h"
#include "game.h"

//
// Initialization
//

Object::Object(Object* creator, ObjectType t)
{

	owner = creator;

	type = t;

	alive = true;
	hps = 1;
	invulnerable = false;
	forceImmune = false;

	// Objects don't bounce much by default
	bounceCoefficient = 0.1f;

}

Object::~Object()
{

	// Free attachments
	for (unsigned int i = 0; i < attachPoints.size(); ++ i)
	{

		for (unsigned int j = 0; j < attachPoints[i].attachments.size(); ++ j)
			SafeDelete(attachPoints[i].attachments[j]);

	}

}

const std::string& Object::GetName() const
{

	return name;

}

void Object::SetName(const std::string& newName)
{

	name = newName;

}

Object* Object::GetOwner() const
{

	return owner;

}

bool Object::IsOwnedBy(Object* object) const
{

	return (owner == object);

}

void Object::SetOwner(Object* object)
{

	owner = object;

}

void Object::SetImage(ImageResource* image)
{

	sprite.SetImage(image);
	sprite.SetCenter((float)image->Image().GetWidth() / 2, (float)image->Image().GetHeight() / 2);

}

void Object::SetPosition(Vec2f v)
{
	
	PhysicsObject::SetPosition(v);

	Moved();

}

void Object::SetPosition(float x, float y)
{
	
	PhysicsObject::SetPosition(x, y);

	Moved();

}

void Object::SetHitpoints(int newHitpoints)
{

	hps = newHitpoints;

	if (hps <= 0)
		Die();

}

void Object::Moved()
{

	sprite.SetPosition(bounds.center.x, bounds.center.y);

}

void Object::Die(bool instant)
{

	alive = false;

}

void Object::Select()
{

	sprite.SetColor(Color(0, 255, 0));
	selected = true;

}

void Object::Deselect()
{

	sprite.SetColor(Color(255, 255, 255));
	selected = false;

}

bool Object::Contains(const Vec2f& position) const
{

	return bounds.Contains(position);

}

bool Object::OnTerrain() const
{

	return GetHeightAboveTerrain() == 0;

}

float Object::GetHeightAboveTerrain(float xOffset) const
{

	float x = Floor(bounds.center.x) + xOffset;
	float bottom = (bounds.center.y - bounds.extents.y) - 1.0f;

	float terrainHeight = 0.0f;

	// Test for terrain across the bounds
	int sx = RoundDownToInt(x - bounds.extents.x);
	int ex = RoundDownToInt(x + bounds.extents.x) + 1;

	for (int i = sx; i < ex; ++ i)
		terrainHeight = Max(terrainHeight, Game::Get()->GetWorld()->GetTerrain()->GetHeightAt(Vec2f((float)i, bounds.center.y)));

	return bottom - terrainHeight;


}

bool Object::OnCollideWithTerrain(const Vec2f& collisionPoint)
{

	// Get terrain normal at the collision point
	Vec2f normal = Game::Get()->GetWorld()->GetNormalForBox(collisionPoint.x, collisionPoint.y, Max(bounds.extents.x, 5.0f) * 4.0f, Max(bounds.extents.y, 5.0f) * 4.0f);

	Renderer::Get()->DrawDebugCircle(collisionPoint, 10.0f, Color::yellow);

	//
	// Bounce
	//

	Vec2f incident = velocity.Normalize();
	float dp = DotProduct(incident, normal);
	Vec2f direction;
	float dampedSpeed;

	if (dp < 0.0f)
	{

		dampedSpeed = velocity.Length() * bounceCoefficient;
		direction = incident - (normal * 2.0f * dp);

		velocity = direction * dampedSpeed;

	}
	else
	{
	
		dampedSpeed = velocity.Length();
		direction = Vec2f(0.0f, 0.0f);

	}

	// Set the object to rest if it is on the terrain and had a small enough velocity
	if ((dampedSpeed < 100.0f) && (OnTerrain()))
		return true;
	else
		bounds.center -= incident;

	return false;

}

void Object::OnCollideWithObject(Object* object)
{

}

void Object::OnHitpointsChanged(int oldValue)
{

}

void Object::SetBounds(float halfWidth, float halfHeight)
{

	bounds.extents.x = halfWidth;
	bounds.extents.y = halfHeight;

}

bool Object::IsInvulnerable() const
{

	return invulnerable;

}

bool Object::IsAlive() const
{

	return alive;

}

int Object::GetHitPoints() const
{

	return hps;

}

void Object::AdjustHitpoints(int change)
{

	if (!invulnerable)
	{

		int oldHps = hps;
		hps += change;

		OnHitpointsChanged(oldHps);

		if (hps <= 0)
			Die();

	}

}

const Sprite& Object::GetSprite() const
{

	return sprite;

}

ObjectType Object::GetType() const
{

	return type;

}

void Object::DebugRender()
{

	Vec2f throwAway;

	if (Game::Get()->GetWorld()->GetTerrain()->BoxCollision(bounds.center.x, bounds.center.y, bounds.GetWidth(), bounds.GetHeight(), throwAway))
	{
		Renderer::Get()->DrawDebugBox(bounds, Color(255, 0, 0, 64));
		Renderer::Get()->DrawDebugLine(throwAway - Vec2f(2.0f, 0.0f), throwAway + Vec2f(2.0f, 0.0f), Color(255, 255, 255, 255));
		Renderer::Get()->DrawDebugLine(throwAway - Vec2f(0.0f, 2.0f), throwAway + Vec2f(0.0f, 2.0f), Color(255, 255, 255, 255));
	}
	else
		Renderer::Get()->DrawDebugBox(bounds, Color(0, 255, 0, 64));

	float height = Game::Get()->GetWorld()->GetTerrain()->GetHeightAt(bounds.center);

	Vec2f ground = Vec2f(bounds.center.x, height);

	Vec2f perpendicular = Game::Get()->GetWorld()->GetTerrain()->GetNormalForBox(ground.x, ground.y, Max(bounds.extents.x, 5.0f) * 2.0f, Max(bounds.extents.y, 5.0f) * 2.0f).Perpendicular();

	if (atRest == true)
		Renderer::Get()->DrawDebugLine(ground - perpendicular * 10.0f, ground + perpendicular * 10.0f, Color::green);
	else
		Renderer::Get()->DrawDebugLine(ground - perpendicular * 10.0f, ground + perpendicular * 10.0f, Color::red);

}

void Object::Render()
{

	// Render object sprite
	Renderer::Get()->Render(sprite);

	// Render attachments
	for (unsigned int i = 0; i < attachPoints.size(); ++ i)
	{

		for (unsigned int j = 0; j < attachPoints[i].attachments.size(); ++ j)
			attachPoints[i].attachments[j]->Render(GetPosition() + attachPoints[i].offset);

	}

}

bool Object::IsForceImmune() const
{

	return forceImmune;

}

void Object::SetForceImmune(bool state)
{

	forceImmune = state;

}

/*
	Helpers
*/

std::string ObjectTypeToString(ObjectType type)
{

	switch (type)
	{

	case ObjectType_Slug:
		return "Slug";

	case ObjectType_Flavor:
		return "Flavor";

	case ObjectType_Pickup:
		return "Pickup";

	case ObjectType_Projectile:
		return "Projectile";

	}

	return "ObjectType_UNKNOWN";

}

void Object::AddAttachpoint(const Vec2f& position)
{

	AttachPoint ap;
	ap.offset = position;
	attachPoints.push_back(ap);

}

void Object::AddAttachment(unsigned int attachPointIndex, Attachment* attachment)
{

	if ((attachPointIndex >= 0) && (attachPointIndex <= attachPoints.size()))
		attachPoints[attachPointIndex].attachments.push_back(attachment);

}