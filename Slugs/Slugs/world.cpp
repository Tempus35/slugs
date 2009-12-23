#include "world.h"
#include "game.h"

#define PB_DEBUG

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
		crosshairSprite = Sprite(ir);

	ir = (ImageResource*)ResourceManager::Get()->GetResource("image_arrow");
	
	if (ir)
		arrowSprite = Sprite(ir);

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
	terrain->Generate(0);

	int spacing = waterImage->Image().GetHeight() / 2;
	int start = waterImage->Image().GetHeight() * WORLD_WATER_LINES / 2;
	for (int i = 0; i < WORLD_WATER_LINES; ++ i)
	{

		if ((i % 2) == 0)
			water[i] = new Water(waterImage, Color(83, 120, 249), 0, i * spacing - start, terrain->WidthInPixels(), Random::RandomFloat(WORLD_WATER_MIN_SPEED, WORLD_WATER_MAX_SPEED));
		else
			water[i] = new Water(waterImage, Color(83, 120, 249), 0, i * spacing - start, terrain->WidthInPixels(), -Random::RandomFloat(WORLD_WATER_MIN_SPEED, WORLD_WATER_MAX_SPEED));
		
	}	

	clouds = new Clouds((ImageResource*)ResourceManager::Get()->GetResource("image_cloud"), Color(150, 150, 150), 0, -terrain->HeightInPixels() + 20, terrain->WidthInPixels(), Random::RandomInt(WORLD_CLOUDS_MIN, WORLD_CLOUDS_MAX), WORLD_CLOUD_LAYERS);

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

void World::Update(float elapsedTime)
{

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

	// Update all alive objects, remove dead ones
	list<Object*>::iterator i = objects.begin();

	while (i != objects.end())
	{

		Object* obj = *i;

		if (obj->IsAlive())
		{

			// Get starting position
			Vec2f lastPos = obj->GetPosition();
			Vec2f collisionPos, freePos;

			// Ensure object is not colliding with terrain
			//ASSERT(!terrain->BoxCollision(lastPos.x, lastPos.y, obj->GetBounds().extents.x * 2.0f, obj->GetBounds().extents.y * 2.0f, collisionPos));

			// Update object
			bool moved = obj->Update(elapsedTime, gravity, wind);

			if ((moved) && (!obj->IsAtRest()))
			{

				// Get new position
				Vec2f pos = obj->GetPosition();

				if (!terrain->Contains(pos.x, pos.y))
				{

					// Kill object instantly, it left the terrain area
            		obj->Die(true);
				
				}
				else
				{

					//
					// Test for collision with the terrain
					//

					if (terrain->BoxCollisionIterated(lastPos.x, lastPos.y, pos.x, pos.y, obj->GetBounds().extents.x * 2.0f, obj->GetBounds().extents.y * 2.0f, collisionPos, freePos))
					{

						// Set back to the position where the collision actually happened
 						obj->SetPosition(collisionPos.x, collisionPos.y);

						// Fire collide event
						if (obj->OnCollideWithTerrain())
							obj->SetAtRest(true);

						// Back up so the object is free
						obj->SetPosition(freePos.x, freePos.y);

						// This should only be triggered if an object is placed in a colliding position
						//ASSERT(!terrain->BoxCollision(freePos.x, freePos.y, obj->GetBounds().extents.x * 2.0f, obj->GetBounds().extents.y * 2.0f, collisionPos));

					}

				}

			}
			else
			{

				// Object is stationary, check to see if we still have terrain holding it up

				//if (!terrain->BoxCollision(lastPos.x, lastPos.y, obj->GetBounds().extents.x * 2.0f + 2, obj->GetBounds().extents.y * 2.0f + 2, throwAway))						
				if (terrain->GetHeightForBox(obj->GetBaseBox()) == -1.0f)			
					obj->SetAtRest(false);

			}

			++i;

		}
		else
		{

			if (selectedObject == *i)
				selectedObject = NULL;

			delete *i;
			i = objects.erase(i);

		}

	}

	// Add all pending objects
	if (pendingObjects.size() > 0)
	{

		for (unsigned int j = 0; j < pendingObjects.size(); ++ j)
			AddObject(pendingObjects[j]);

		pendingObjects.clear();

	}

	// Update terrain
	terrain->BufferDirty();

}

void World::AddObject(Object* object)
{

	if (object)
		objects.insert(objects.end(), object);

}

void World::AddCreatedObject(Object* object)
{

	if (object)
		pendingObjects.insert(pendingObjects.end(), object);

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
	int offCenterY = RoundToInt(newPosition.y + terrain->HeightInPixels() / 2.0f);
	int halfXDifference, halfYDifference;

	size = backgroundSprites[0].GetSize();
	halfXDifference = ((int)size.x - terrain->WidthInPixels()) / 2;
	halfYDifference = ((int)size.y - terrain->HeightInPixels()) / 2;

	backgroundSprites[0].SetPosition((float)(offCenterX / 2 - halfXDifference), (float)(offCenterY / 2 - terrain->HeightInPixels() + halfYDifference));

	size = backgroundSprites[1].GetSize();
	halfXDifference = RoundToInt((size.x - terrain->WidthInPixels()) / 2.0f);
	halfYDifference = RoundToInt((size.y - terrain->HeightInPixels()) / 2.0f);

	backgroundSprites[1].SetPosition((float)(offCenterX / 4 - halfXDifference), float(offCenterY / 4 - terrain->HeightInPixels() + halfYDifference));

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

		if ((*i)->Contains(position.x, position.y))
		{

			obj = (*i);
			break;

		}

	}

	return obj;

}

void World::SimulateExplosion(float x, float y, float strength, float forceMultiplier)
{

	// Update the terrain
	terrain->ClearCircle(x, y, strength, WORLD_EXPLOSION_BORDER);

	//
	// Add forces and damage to objects
	//

	float dx, dy, d;
	float hitStrength, hitPower;
	Vec2f pos;
	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		// Ignore dead objects
		if (!obj->IsAlive())
			continue;

		// Ignore invulnerable objects
		if (obj->IsInvulnerable())
			continue;

		pos = obj->GetPosition();
		dx = pos.x - x;
		dy = pos.y - y;
		d = sqrtf(dx * dx + dy * dy);

		if (d <= strength * forceMultiplier)
		{
	
			// Determine hit strength and adjust object hitpoints
			hitStrength = 1.0f - (d / (float)strength);
			hitPower = hitStrength * strength;
			obj->AdjustHitpoints(-(int)hitPower);

			// Add force
			obj->SetAtRest(false);
			obj->SetVelocity(CopySign(dx, hitPower) * forceMultiplier, CopySign(dy, hitPower) * forceMultiplier);

		}

	}

}

void World::DeferExplosion(float x, float y, float strength, float forceMultiplier)
{

	deferredExplosions.push_back(DeferredExplosion(x, y, strength, forceMultiplier));

}

void World::SimulateExplosions()
{

	for (unsigned int i = 0; i < deferredExplosions.size(); ++ i)
		SimulateExplosion(deferredExplosions[i].x, deferredExplosions[i].y, deferredExplosions[i].strength, deferredExplosions[i].forceMultiplier);

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

	Game::Get()->GetFXManager()->Render();

	// Render clouds
	clouds->Render();

	// Render Background Water
	for (int i = 0; i < WORLD_WATER_LINES / 2; ++ i)
		water[i]->Render();

	// Render terrain
	if (terrain)
		terrain->Render();

	// Render objects
	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		if (obj->IsAlive())
		{

			const Sprite& s = obj->GetSprite();
			renderer->Render(obj->GetSprite());

			#ifdef PB_DEBUG
				renderer->Render(obj->marker);
			#endif

			if ((selectedObject == obj) && (obj->GetType() == ObjectType_Slug))
			{

				// Aiming crosshair/arrow
				Slug* slugObj = (Slug*)obj;
				
				Vec2f size;
				Vec2f objPos = slugObj->GetPosition();
				float angle = slugObj->GetAimAngle();

				size = crosshairSprite.GetSize();

				if (slugObj->GetFacingDirection() == FACINGDIRECTION_RIGHT)
					crosshairSprite.SetPosition(objPos.x + cosf(angle) * WORLD_CROSSHAIR_DISTANCE - size.x / 2, -objPos.y - sinf(angle) * WORLD_CROSSHAIR_DISTANCE - size.y / 2);
				else
					crosshairSprite.SetPosition(objPos.x - cosf(angle) * WORLD_CROSSHAIR_DISTANCE - size.x / 2, -objPos.y - sinf(angle) * WORLD_CROSSHAIR_DISTANCE - size.y / 2);
				
				size = arrowSprite.GetSize();
				arrowSprite.SetPosition(objPos.x - size.x / 2, objPos.y - WORLD_ARROW_DISTANCE - size.y / 2);				

				renderer->Render(crosshairSprite);
				//renderer->Render(arrowSprite);

			}

		}

	}

	// Render Foreground Water
	for (int i = WORLD_WATER_LINES / 2; i < WORLD_WATER_LINES; ++ i)
		water[i]->Render();

	/*
	// Debug Bounds
	#ifdef PB_DEBUG

		sf::Shape bounds;
		bounds.EnableFill(false);
		bounds.SetOutlineWidth(1.0f);
		bounds.AddPoint(0, 0, sf::Color(0, 0, 0), sf::Color(255, 255, 255));
		bounds.AddPoint((float)terrain->WidthInPixels(), 0, sf::Color(0, 0, 0), sf::Color(255, 255, 255));
		bounds.AddPoint((float)terrain->WidthInPixels(), -(float)terrain->HeightInPixels(), sf::Color(0, 0, 0), sf::Color(255, 255, 255));
		bounds.AddPoint(0, -(float)terrain->HeightInPixels(), sf::Color(0, 0, 0), sf::Color(255, 255, 255));
		renderer.Draw(bounds);

	#endif
	*/

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

World::IntersectionType World::GetRayIntersection(const Vec2f& start, const Vec2f& direction, Vec2f& collisionPos, Object* ignore)
{

	float maxRange = 1000000.0f;
	float maxX, maxY;

	if (direction.x > 0)
		maxX = terrain->WidthInPixels() - start.x;
	else
		maxX = start.x;

	if (direction.y > 0)
		maxY = terrain->HeightInPixels() - start.y;
	else
		maxY = start.y;

	maxRange = Sqrt(maxX * maxX + maxY * maxY);

	Vec2f intersectionPos;

	//
	// Check the terrain
	//
	
	IntersectionType collision = IntersectionType_None;
	
	if (terrain->RayIntersection(start, direction, maxRange, intersectionPos))
	{

		collisionPos = intersectionPos;
		maxRange = (intersectionPos - start).Length();

		collision = IntersectionType_Terrain;

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
		if (RayCircleIntersection(start, direction, obj->GetPosition(), Max(obj->GetBounds().extents.x, obj->GetBounds().extents.y), maxRange, intersectionPos))
		{

			maxRange = (intersectionPos - start).Length();
			collisionPos = intersectionPos;
			collision = IntersectionType_Object;

		}

	}

	return collision;

}

Vec2f World::GetNormal(const Vec2f& position) const
{

	return terrain->NormalAtPoint((int)position.x, (int)position.y, NULL);

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

	const float blockRadius = 50.0f;

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