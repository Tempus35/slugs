#include "world.h"
#include "game.h"

//#define PB_DEBUG

/*
	class WorldBuilderThread
*/

unsigned int WorldBuilderThread::DoWork(void* object)
{

	((Terrain*)object)->Generate(0);

	return 0;

}

/*
	class World
*/

World::World()
{

	terrain = NULL;

	gravity = Vec2f(0, WORLD_DEFAULT_GRAVITY);
	wind = Vec2f(0, 0);

	selectedObject = NULL;

	ImageResource* ir;
	
	ir = (ImageResource*)ResourceManager::Get()->GetResource("image_crosshair");

	if (ir)
	{

		crosshairSprite = Sprite(ir);
		crosshairSprite.SetCenter(crosshairSprite.GetWidth() / 2, crosshairSprite.GetHeight() / 2);

	}

	//ir = (ImageResource*)ResourceManager::Get()->GetResource("image_arrow");
	
	//if (ir)
	//	arrowSprite = Sprite(ir);

	crosshairVisible = false;

}

World::~World()
{

	// Delete terrain
	if (terrain)
		delete terrain;

}

//
// Setup
//

void World::Build(int width, int height, TextureBuffer* groundTexture, TextureBuffer* overTexture, TextureBuffer* underTexture, ImageResource* waterImage)
{

	// Create the terrain, set art and generate
	terrain = new Terrain(width, height);
	terrain->SetArt(groundTexture, overTexture, underTexture);

	int spacing = 32;
	int start = 64 * WORLD_WATER_LINES / 2;
	for (int i = 0; i < WORLD_WATER_LINES; ++ i)
	{

		if ((i % 2) == 0)
			water[i] = new Water(waterImage, Color(83, 120, 249, 128), 0, i * spacing - start, terrain->WidthInPixels(), Random::RandomFloat(WORLD_WATER_MIN_SPEED, WORLD_WATER_MAX_SPEED));
		else
			water[i] = new Water(waterImage, Color(83, 120, 249, 128), 0, i * spacing - start, terrain->WidthInPixels(), -Random::RandomFloat(WORLD_WATER_MIN_SPEED, WORLD_WATER_MAX_SPEED));
		
	}	

	waterHeight = (float)start * 0.5f;

	clouds = new Clouds((ImageResource*)ResourceManager::Get()->GetResource("image_cloud"), Color(150, 150, 150), 0, -terrain->HeightInPixels() + 20, terrain->WidthInPixels(), Random::RandomInt(WORLD_CLOUDS_MIN, WORLD_CLOUDS_MAX), WORLD_CLOUD_LAYERS);

	// Build the terrain on the worker thread
	ASSERT(!workerThread.IsRunning());
	workerThread.Start(terrain);

}

void World::Regenerate()
{

	RemoveAllObjects();

	if (terrain)
		terrain->Generate(0);

}

//
// Simulation
//

bool World::Update(float elapsedTime)
{

	//
	// Delete all dead objects
	// This is done first so that anything tracking an object will have had chance to stop doing so on the last frame, before the object is deleted
	//

	list<Object*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		Object* obj = (*i);

		if (!obj->IsAlive())
		{

			if (selectedObject == obj)
				selectedObject = NULL;

			// Don't delete slugs, they are owned by their teams
			if (obj->GetType() != ObjectType_Slug)
				SafeDelete(obj);

			i = objects.erase(i);

		}
		else
			i ++;

	}

	// Flag used to determine if anything happened this update
	bool somethingMoved = false;

	// Deselect our selected slug if it is dying
	if ((selectedObject) && (selectedObject->GetHitPoints() <= 0))
	{

		selectedObject->Deselect();
		selectedObject = NULL;

	}

	// Update water
	for (int i = 0; i < WORLD_WATER_LINES; ++ i)
		water[i]->Update(elapsedTime);

	// Update clouds
	clouds->Update(elapsedTime);

	//
	// Update objects
	//

	i = objects.begin();
	list<Object*>::iterator j = objects.begin(), k;

	while (i != objects.end())
	{

		Object* obj = *i;

		if (obj->IsAlive())
		{

			// Get starting position
			Vec2f lastPos = obj->GetPosition();
			Vec2f collisionPos, freePos;
			
			Vec2f throwAway;
			//ASSERT(!terrain->BoxCollision(lastPos.x, lastPos.y, obj->GetBounds().GetWidth(), obj->GetBounds().GetHeight(), throwAway));

			// If this is a slug, check for drowning
			if ((obj->GetType() == ObjectType_Slug) && (lastPos.y <= waterHeight))
				obj->SetHitpoints(0);

			// Update object
			bool moved = obj->Update(elapsedTime, gravity, wind);
			somethingMoved |= moved;

			if ((moved) && (!obj->IsAtRest()))
			{

				// Get new position
				Vec2f pos = obj->GetPosition();

				if (!terrain->ContainsOpenTop(pos.x, pos.y))
				{

					// Kill object instantly, it left the terrain area
            		obj->Die(true);
				
				}
				else
				{

					//
					// Test for collision with the terrain
					//

					const Boxf& boundingBox = obj->GetBounds();

					bool collision = terrain->BoxCollisionIterated(lastPos.x, lastPos.y, pos.x, pos.y, boundingBox.GetWidth(), boundingBox.GetHeight(), collisionPos, freePos);
					
					if (collision)
					{

						// Back up so the object is free
						obj->SetPosition(freePos.x, freePos.y);

						Vec2f newPos = obj->GetPosition();
						// ASSERT(!terrain->BoxCollision(newPos.x, newPos.y, obj->GetBounds().GetWidth(), obj->GetBounds().GetHeight(), throwAway));

						// Fire collide event
						if (obj->OnCollideWithTerrain(collisionPos))
							obj->SetAtRest(true);

					}

				}

			}
			else if (obj->IsAtRest())
			{

				// If the object is in the resting state but is not supported by terrain, let it fall
				if (!obj->OnTerrain())
					obj->SetAtRest(false);

			}

			//
			// Check for collisions with objects
			//

			k = j;

			if (j != objects.end())
				j++;

			while (k != objects.end())
			{

				Object* collider = (Object*)(*k);

				if ((obj != collider) && (!collider->IsOwnedBy(obj)) && (!obj->IsOwnedBy(collider)))
				{

					if (BoxBoxIntersection(obj->GetBounds(), collider->GetBounds()) == true)
					{

						obj->OnCollideWithObject(collider);
						collider->OnCollideWithObject(obj);

					}

				}

				++ k;

			}

		}

		++i;

	}

	//
	// Add all pending objects
	//

	if (pendingObjects.size() > 0)
	{

		for (unsigned int j = 0; j < pendingObjects.size(); ++ j)
			AddObject(pendingObjects[j]);

		pendingObjects.clear();

	}

	// Update terrain
	terrain->BufferDirty();

	return somethingMoved;

}

void World::AddObject(Object* object)
{

	if (object)
		objects.insert(objects.end(), object);

}

void World::AddCreatedObject(Object* object)
{

	if (object)
		pendingObjects.push_back(object);

}

void World::RemoveAllObjects()
{

	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;
		delete obj;

	}

	objects.clear();

}

void World::SetWaterColor(Color& color)
{

	for (int i = 0; i < WORLD_WATER_LINES; ++ i)
		water[i]->SetColor(color);

}

void World::SetBackground(ImageResource* farImage, ImageResource* nearImage)
{

	sf::Image& fim = farImage->Image();
	sf::Image& nim = nearImage->Image();

	backgroundSprites[0].SetAlphaBlend(false);
	backgroundSprites[0].SetImage(farImage);
	backgroundSprites[1].SetImage(nearImage);

	float scaleFactorA, scaleFactorB;
	scaleFactorA = (float)terrain->WidthInPixels() / (float)fim.GetWidth();
	scaleFactorB = (float)terrain->HeightInPixels() / (float)fim.GetHeight();

	if (scaleFactorA > scaleFactorB)
		backgroundSprites[0].SetScale(scaleFactorA, scaleFactorA);
	else
		backgroundSprites[0].SetScale(scaleFactorB, scaleFactorB);

	scaleFactorA = (float)terrain->WidthInPixels() / (float)nim.GetWidth();
	scaleFactorB = (float)terrain->HeightInPixels() / (float)nim.GetHeight();

	if (scaleFactorA > scaleFactorB)
		backgroundSprites[1].SetScale(scaleFactorA, scaleFactorA);
	else
		backgroundSprites[1].SetScale(scaleFactorB, scaleFactorB);


}

void World::CameraMoved(const Vec2f& newPosition)
{

	Vec2f size;

	//
	// Get the distance from the center of the terrain, calculate parallax params
	// and adjust parallax by moving the background images
	//

	int offCenterX = RoundToInt(newPosition.x - terrain->WidthInPixels() / 2.0f);
	int offCenterY = RoundToInt(-newPosition.y + terrain->HeightInPixels() / 2.0f);
	int halfXDifference, halfYDifference;

	size = backgroundSprites[0].GetSize();
	halfXDifference = ((int)size.x - terrain->WidthInPixels()) / 2;
	halfYDifference = ((int)size.y - terrain->HeightInPixels()) / 2;

	backgroundSprites[0].SetPosition((float)(offCenterX / 2 - halfXDifference), -(float)(offCenterY / 2 - terrain->HeightInPixels() + halfYDifference));

	size = backgroundSprites[1].GetSize();
	halfXDifference = RoundToInt((size.x - terrain->WidthInPixels()) / 2.0f);
	halfYDifference = RoundToInt((size.y - terrain->HeightInPixels()) / 2.0f);

	backgroundSprites[1].SetPosition((float)(offCenterX / 4 - halfXDifference), -(float)(offCenterY / 4 - terrain->HeightInPixels() + halfYDifference));

}

Object* World::SelectObjectAtPosition(const Vec2f& position)
{
	
	Object* object = GetObjectAtPosition(position);
	SelectObject(object);

	return object;

}

void World::SelectObject(Object* object)
{

	// Deselect any currently selected object
	if (selectedObject)
		selectedObject->Deselect();

	// Update selection
	selectedObject = object;
	
	if (selectedObject)
		selectedObject->Select();

}

Object* World::GetObjectAtPosition(const Vec2f& position)
{

	//
	// Find the object at the given point
	//

	Object* obj = NULL;

	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		if ((*i)->Contains(position))
		{

			obj = (*i);
			break;

		}

	}

	return obj;

}

void World::SimulateExplosion(const Vec2f& position, const ExplosionData& data)
{

	// Remove terrain in the explosion radius
	terrain->ClearCircle(position, data.explosionRadius, WORLD_EXPLOSION_BORDER);

	//
	// Add forces and damage to objects
	//

	float dx, dy, d;
	Vec2f pos;
	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		// Ignore dead objects
		if (!obj->IsAlive())
			continue;

		// Calculate distance to explosion
		pos = obj->GetPosition();
		dx = pos.x - position.x;
		dy = pos.y - position.y;
		d = sqrtf(dx * dx + dy * dy);

		if ((!obj->IsForceImmune()) && (d <= data.forceRadius))
		{

			// Force falls off from the full amount at the center of the explosion to half the amount at the maximum force distance
			float force;
			
			if (data.noFalloff)
				force = 1.0f;
			else
				force = 1.0f - (d / (data.forceRadius * 2.0f));

			// Add force
			obj->SetVelocity(CopySign(dx, force) * data.forceStrength, CopySign(dy, force) * data.forceStrength);
			obj->SetAtRest(false);

		}

		if (d <= data.damageRadius)
		{

			float hitPower;
			
			if (data.noFalloff)
				hitPower = 1.0f;
			else
				hitPower = 1.0f - (d / data.damageRadius);

			// Damage object
			obj->AdjustHitpoints(-RoundDownToInt(hitPower * data.damageStrength));

		}

	}

}

void World::DeferExplosion(const Vec2f& position, const ExplosionData& data)
{

	deferredExplosions.push_back(DeferredExplosion(position, data));

}

void World::SimulateExplosions()
{

	//
	// Process all deferred explosions
	//

	for (unsigned int i = 0; i < deferredExplosions.size(); ++ i)
		SimulateExplosion(deferredExplosions[i].position, deferredExplosions[i].data);

	deferredExplosions.clear();

}

//
// Rendering
//

void World::Render()
{

	Renderer* renderer = Renderer::Get();

	// Render backgrounds
	renderer->Render(backgroundSprites[0]);
	renderer->Render(backgroundSprites[1]);

	// Render clouds
	clouds->Render();

	// Render Background Water
	for (int i = WORLD_WATER_LINES / 2 - 1; i >= 0; -- i)
		water[i]->Render();

	// Render terrain
	if (terrain)
		terrain->Render();

	// Render objects
	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		#ifdef _DEBUG

		if (Game::Get()->GetGameBool(GameBool_Debug))
			obj->DebugRender();

		#endif

		if (obj->IsAlive())
		{

			obj->Render();

			if (obj->GetType() == ObjectType_Slug)
			{

				Slug* slugObj = (Slug*)obj;
				Vec2f objPos = slugObj->GetPosition();

				if (Game::Get()->GetGameBool(GameBool_AlwaysShowNames))
					Renderer::Get()->RenderTextShadowed(objPos.x, -objPos.y - 34.0f, (FontResource*)ResourceManager::Get()->GetResource("font_copacetix"), slugObj->GetName(), 16.0f, slugObj->GetTeam()->GetColor(), Color(0, 0, 0), FontFlag_Bold|FontFlag_Centered);

				if (Game::Get()->GetGameBool(GameBool_AlwaysShowHps))
				{

					char hps[16];
					sprintf_s(hps, 16, "%i", Max(0, slugObj->GetHitPoints()));
					Renderer::Get()->RenderTextShadowed(objPos.x, -objPos.y - 20.0f, (FontResource*)ResourceManager::Get()->GetResource("font_copacetix"), hps, 8.0f, slugObj->GetTeam()->GetColor(), Color(0, 0, 0), FontFlag_Bold|FontFlag_Centered);

				}

			}

		}

	}

	// Render Foreground Water
	for (int i = WORLD_WATER_LINES - 1; i >= WORLD_WATER_LINES / 2; -- i)
		water[i]->Render();

	// Render FX
	Game::Get()->GetFXManager()->Render();

	// Render Crosshair
	if (crosshairVisible)
		Renderer::Get()->Render(crosshairSprite);

}

//
// Accessors
//

Vec2f World::Gravity()
{

	return gravity;

}

Vec2f World::Wind()
{

	return wind;

}

int World::WidthInPixels()
{

	return terrain->WidthInPixels();

}

int World::HeightInPixels()
{

	return terrain->HeightInPixels();

}

unsigned int World::Seed()
{

	return terrain->Seed();

}

void World::SetGravity(Vec2f newGravity)
{

	gravity = newGravity;

}

void World::SetWind(Vec2f newWind)
{

	wind = newWind;

}

Object* World::SelectedObject()
{

	return selectedObject;

}

Intersection World::GetLineIntersection(const Vec2f& start, const Vec2f& end, Object* ignore)
{

	Vec2f direction = end - start;
	return GetRayIntersection(start, direction.Normalize(), ignore, direction.Length());

}

bool World::GetCircleIntersection(const Vec2f& center, float radius)
{

	// Check terrain
	if (terrain->CircleCollision(center, radius))
		return true;

	return false;

}

Intersection World::GetRayIntersection(const Vec2f& start, const Vec2f& direction, Object* ignore, float range)
{

	//
	// Clamp the range to the terrain
	//

	Vec2f toEdge;

	if (direction.x > 0)
		toEdge.x = terrain->WidthInPixels() - start.x;
	else
		toEdge.x = start.x;

	if (direction.y > 0)
		toEdge.y = terrain->HeightInPixels() - start.y;
	else
		toEdge.y = start.y;

	float maxRange = Min(toEdge.Length(), range);

	//
	// Check the terrain
	//

	Intersection closestIntersection, intersection;
	
	if (terrain->RayIntersection(start, direction, maxRange, closestIntersection.position))
	{

		closestIntersection.distance = (closestIntersection.position - start).Length();
		closestIntersection.type = IntersectionType_Terrain;

	}

	//
	// Check objects
	//

	for (std::list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		// Ignore the passed in object
		if (obj == ignore)
			continue;

		// Ignore dead objects
		if (!obj->IsAlive())
			continue;

		// Test for an intersection
		if (RayCircleIntersection(start, direction, obj->GetPosition(), Max(obj->GetBounds().extents.x, obj->GetBounds().extents.y), closestIntersection.distance, intersection.position))
		{

			closestIntersection.distance = (intersection.position - start).Length();
			closestIntersection.position = intersection.position;
			closestIntersection.object = obj;
			closestIntersection.type = IntersectionType_Object;

		}

	}

	return closestIntersection;

}

Vec2f World::GetNormalForBox(float centerX, float centerY, float width, float height) const
{

	return terrain->GetNormalForBox(centerX, centerY, width, height);

}

Terrain* World::GetTerrain() const
{

	return terrain;

}

void World::GetSpawnPoints(std::vector<Vec2f>& list, int count)
{

	const float blockRadius = 75.0f;

	for (int i = 0; i < count; ++ i)
		list.push_back(GetSpawnPointAndBlock(blockRadius));

}

Vec2f World::GetSpawnPointAndBlock(float blockRadius)
{

	int numAttempts = 100;

	Vec2f spawnPosition;

	while (numAttempts > 0)
	{

		spawnPosition = terrain->GetSpawnPoint();

		if (!IsBlocked(spawnPosition))
		{

			//
			// Add the block
			//

			Circle block;
			block.center = spawnPosition;
			block.radius = blockRadius;
			areaBlocks.push_back(block);

			// Return the position
			return spawnPosition;

		}

		numAttempts --;

	}

	ASSERTMSG(0, "Failed to find valid spawn position!");

	return Vec2f(0, 0);

}

bool World::IsBlocked(const Vec2f& position)
{

	for (unsigned int i = 0; i < areaBlocks.size(); ++ i)
	{

		if (areaBlocks[i].Contains(position))
			return true;

	}

	return false;

}

void World::ClearBlocks()
{

	areaBlocks.clear();

}

// Gets the nearest object of a given type to an object
Object* World::GetNearestObject(Object* object, ObjectType type)
{

	ASSERT(object != NULL);

	Object* closestObject = NULL;
	float closestDistance = Math::INFINITY;
	Vec2f objectPosition = object->GetPosition();

	std::list<Object*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		Object* obj = (*i);

		if ((obj != object) && ((type == ObjectType_Any) || (obj->GetType() == type)))
		{

			float distance = (obj->GetPosition() - objectPosition).LengthSquared();

			if (distance < closestDistance)
			{

				closestObject = obj;
				closestDistance = distance;

			}

		}

		i ++;
			
	}

	return closestObject;

}

// Gets all objects within a given radius of an object
void World::GetObjectsNear(std::vector<Object*>& list, Object* object, float radius, ObjectType type)
{

	ASSERT(object != NULL);

	return GetObjectsNear(list, object->GetPosition(), radius, type, object);

}

void World::GetObjectsNear(std::vector<Object*>& list, const Vec2f& point, float radius, ObjectType type, Object* ignore)
{

	ASSERT(radius > 0.0f);

	float radiusSquared = Sqr(radius);

	std::list<Object*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		Object* obj = (*i);

		if ((obj != ignore) && ((type == ObjectType_Any) || (obj->GetType() == type)))
		{

			if ((obj->GetPosition() - point).LengthSquared() <= radiusSquared)
				list.push_back(obj);

		}

		i ++;
			
	}

}

Object* World::FindObject(const Vec2f& position, float radius)
{

	float radiusSquared = Sqr(radius);

	std::list<Object*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		Object* obj = (*i);

		if ((obj->GetPosition() - position).LengthSquared() <= radiusSquared)
			return obj;

		i ++;
			
	}

	return NULL;

}

bool World::ObjectCanSeeDirect(Object* from, Object* to)
{

	Vec2f intersectionPos;
	Intersection intersection = GetLineIntersection(from->GetPosition(), to->GetPosition(), from);

	return (intersection.object == to);

}

bool World::ObjectCanShootIndirect(Object* from, Object* to, float maxSpeed, Vec2f& optimalDirection, float& optimalSpeed)
{

	//
	// Calculate the two launch angles that will hit the target
	//

	float g = -gravity.Length();
	const Vec2f& start = from->GetPosition();

	Vec2f directions[2];

	bool canShoot = false;

	const float TEST_SPEED_STEP = 50.0f;
	float testSpeed = 300.0f;

	// Start a max speed and reduce until we find a valid path
	while (!canShoot)
	{
	
		canShoot = CalculateLaunchDirection2(start, to->GetPosition(), testSpeed, -g, directions[0], directions[1]);
		optimalSpeed = testSpeed;

		testSpeed += TEST_SPEED_STEP;

		if (testSpeed > 1500.0f)
			break;

	}	

	if (canShoot)
	{

		//
		// Test the path for intersections with the terrain and objects
		// The lower direction is tested first
		//

		const float sample = 0.0125f;

		for (int i = 0; i < 2; ++ i)
		{

			Vec2f position = start;

			float t = sample;
			Vec2f velocity = directions[i] * optimalSpeed;

			Vec2f intersection;

			while (position.y >= 0.0f)
			{

				position.x = start.x + velocity.x * t;
				position.y = start.y + velocity.y * t + 0.5f * g * t * t;

				if (Game::Get()->GetGameBool(GameBool_Debug))
					Renderer::Get()->DrawDebugBox(Boxf(position, Vec2f(2.5f, 2.5f)), Color::purple);

				if (terrain->BoxCollision(position.x, position.y, 5.0f, 5.0f, intersection))
					return false;

				Object* object = FindObject(position, 5.0f);

				if (object)
				{

					if (object == to)
					{
					
						optimalDirection = directions[i];
						return true;
					
					}
					else if (object != from)
						break;

				}

				t += sample;

			}

		}

	}

	return false;

}

void World::SetCrosshairPosition(const Vec2f& position)
{

	crosshairSprite.SetPosition(position.x, position.y);

}

void World::SetCrosshairVisible(bool state)
{

	crosshairVisible = state;

}

void World::DropCrate()
{

	const float REQUIRED_RADIUS = 100.0f;

	int numAttempts = 20;
	Vec2f dropPoint;
	std::vector<Object*> objects;
	bool found = false;

	// Find a suitable drop location
	for (int i = 0; i < numAttempts; ++ i)
	{

		dropPoint = terrain->GetDropPoint();
		GetObjectsNear(objects, dropPoint, REQUIRED_RADIUS);

		if (objects.size() == 0)
		{

			found = true;
			break;

		}
		else
			objects.clear();

	}

	if (found)
	{

		//
		// Pick and create a random crate
		//

		float pick = Random::RandomFloat();

		Pickup* crate;

		if (pick < 0.25f)
			crate = new Pickup_Health();
		else if (pick < 0.5f)
			crate = new Pickup_Exploding();
		else
			crate = new Pickup_Weapon();

		// Add the crate to the world

		if (crate)
		{

			crate->SetPosition(dropPoint.x, (float)HeightInPixels() + 50.0f);
			crate->SetBounds(16.0f, 16.0f);
			crate->SetImage(ResourceManager::Get()->GetImage("image_crate"));
			AddCreatedObject(crate);

		}

	}

}

bool World::IsBuilding() const
{

	return workerThread.IsRunning();

}

void World::CancelBuilding()
{

	if (workerThread.IsRunning())
		workerThread.Stop();

	// TODO: This doesn't actually cancel anything since the thread doesn't check for stop events at the moment

}