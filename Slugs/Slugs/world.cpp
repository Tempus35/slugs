#include "world.h"

#define PB_DEBUG

/*
	class World
*/

World::World()
{

	terrain = NULL;

	gravity = Vector2(0, WORLD_DEFAULT_GRAVITY);
	wind = Vector2(0, 0);

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
			Vector2 lastPos = obj->GetPosition();

			// Update object
			obj->Update(elapsedTime, terrain, gravity, wind);

			// Get new position
			Vector2 pos = obj->GetPosition();

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

				Vector2 collisionPos;
				if (terrain->SquareCollisionIterated((int)lastPos.x, (int)lastPos.y, (int)pos.x, (int)pos.y, 2 * obj->GetRadius(), 2 * obj->GetRadius(), &collisionPos))
				{

					obj->SetPosition(collisionPos.x, collisionPos.y);
					obj->OnCollideWithTerrain(terrain);
					obj->SetAtRest(true);

				}
				else
				{

					// We didn't collide, start moving if we aren't already
					obj->SetAtRest(false);

				}

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

void World::CameraMoved(const Vector2& newPosition)
{

	Vector2 size;

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

Object* World::SelectObjectAtPosition(Vector2 point)
{
	
	Object* object = GetObjectAtPosition(point);
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

Object* World::GetObjectAtPosition(const Vector2& position)
{

	//
	// Find the object at the given point
	//

	Object* obj = NULL;

	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		int pix = (int)position.x;
		int piy = (int)position.y;

		if ((*i)->Contains(pix, piy))
		{

			obj = (*i);
			break;

		}

	}

	return obj;

}

void World::SimulateExplosion(int x, int y, int strength)
{

	// Update the terrain
	terrain->ClearCircle(x, y, strength, WORLD_EXPLOSION_BORDER);

	//
	// Add forces and damage to objects
	//

	float dx, dy, d;
	float hitStrength, hitPower;
	Vector2 pos;
	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		pos = obj->GetPosition();
		dx = pos.x - x;
		dy = pos.y - y;
		d = sqrtf(dx * dx + dy * dy);

		if (d <= strength)
		{
	
			// Determine hit strength and adjust object hitpoints
			hitStrength = 1.0f - (d / (float)strength);
			hitPower = hitStrength * strength;
			obj->AdjustHitpoints(-(int)hitPower);

			// Add force
			obj->SetAtRest(false);
			obj->SetVelocity(CopySign(dx, hitPower) * WORLD_EXPLOSION_MULTIPLIER, CopySign(dy, hitPower) * WORLD_EXPLOSION_MULTIPLIER);

		}

	}

}

void World::DeferExplosion(int x, int y, int strength)
{

	deferredExplosions.push_back(DeferredExplosion(x, y, strength));

}

void World::SimulateExplosions()
{

	for (unsigned int i = 0; i < deferredExplosions.size(); ++ i)
		SimulateExplosion(deferredExplosions[i].x, deferredExplosions[i].y, deferredExplosions[i].strength);

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

	FXManager::Get()->Render();

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
				
				Vector2 size;
				Vector2 objPos = slugObj->GetPosition();
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

Vector2 World::Gravity()
{

	return gravity;

}

Vector2 World::Wind()
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

void World::SetGravity(Vector2 newGravity)
{

	gravity = newGravity;

}

void World::SetWind(Vector2 newWind)
{

	wind = newWind;

}

Object* World::SelectedObject()
{

	return selectedObject;

}

bool World::GetRayIntersection(const Vector2& start, const Vector2& direction, Vector2& collisionPos, Object* ignore)
{

	float maxRange = 1000000.0f;
	Vector2 intersectionPos;

	//
	// Check the terrain
	//
	
	bool collision = terrain->RayIntersection(start, direction, maxRange, intersectionPos);

	if (collision)
	{

		collisionPos = intersectionPos;
		maxRange = VectorLength(intersectionPos - start);

	}

	//
	// Check objects
	//

	for (list<Object*>::iterator i = objects.begin(); i != objects.end(); ++ i)
	{

		Object* obj = *i;

		// Ignore the passed in object
		if (obj == ignore)
			continue;

		// Ignore dead objects
		if (!obj->IsAlive())
			continue;

		// Test for an intersection
		if (RayCircleIntersection(start, direction, obj->GetPosition(), (float)obj->GetRadius(), maxRange, intersectionPos))
		{

			maxRange = VectorLength(intersectionPos - start);
			collisionPos = intersectionPos;
			collision = true;

		}

	}

	return collision;


}