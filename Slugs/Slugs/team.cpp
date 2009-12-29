#include "team.h"
#include "game.h"
#include "aicontroller.h"

Team::Team(Player* player)
{

	ASSERT(player != NULL);

	owner = player;

	name = "";
	color = Color(128, 0, 0);

	weaponStore = NULL;

}

Team::~Team()
{

	// Free slugs
	for (unsigned int i = 0; i < slugs.size(); ++ i)
		SafeDelete(slugs[i]);

	// Free weapon store
	SafeDelete(weaponStore);

}

const std::string& Team::GetName() const
{

	return name;

}

void Team::SetName(const std::string& newName)
{

	name = newName;

}

const Color& Team::GetColor() const
{

	return color;

}

void Team::SetColor(const Color& newColor)
{

	color = newColor;

}

Player* Team::GetPlayer() const
{

	return owner;

}

bool Team::Contains(Slug* slug) const
{

	for (unsigned int i = 0; i < slugs.size(); ++ i)
	{

		if (slugs[i] == slug)
			return true;

	}

	return false;

}

bool Team::Add(Slug* slug)
{

	ASSERT(slug);

	if (!Contains(slug))
	{

		// Add the slug to the team
		slugs.push_back(slug);

		return true;

	}

	return false;

}

void Team::SetWeapons(WeaponStore* store)
{

	SafeDelete(weaponStore);

	weaponStore = store;

	// Assign the weapon store to the slugs on the team
	for (unsigned int i = 0; i < slugs.size(); ++ i)
		slugs[i]->SetWeapons(store);

}

void Team::Randomize(int numSlugs)
{

	// Get random team name
	name = ResourceManager::Get()->GetText("text_teamnames")->GetRandomLine(true);

	std::string colorString = ResourceManager::Get()->GetText("text_colors")->GetRandomLine(true);
	color = Color(colorString);

	TextResource* slugNames = ResourceManager::Get()->GetText("text_slugnames");

	for (int i = 0; i < numSlugs; ++ i)
	{

		AIController* controller;
	
		if (owner->GetType() == PlayerType_Computer)
			controller = new AIController();
		else
			controller = NULL;

		Slug* slug = new Slug(this, controller);

		slug->SetName(slugNames->GetRandomLine(true));

		Add(slug);

	}

}

void Team::GetSlugs(std::vector<Slug*>& list, bool aliveOnly)
{

	for (unsigned int i = 0; i < slugs.size(); ++ i)
	{

		if ((!aliveOnly) || (slugs[i]->GetHitPoints() > 0))
			list.push_back(slugs[i]);

	}

}

void Team::PlaceInWorld()
{

	std::vector<Vec2f> spawnPoints;
	Game::Get()->GetWorld()->GetSpawnPoints(spawnPoints, slugs.size());

	ASSERT(spawnPoints.size() == slugs.size());

	for (unsigned int i = 0; i < slugs.size(); ++ i)
	{

		slugs[i]->SetPosition(spawnPoints[i]);
		Game::Get()->GetWorld()->AddObject(slugs[i]);

	}

}

int Team::GetTotalHitpoints() const
{

	int total = 0;

	for (unsigned int i = 0; i < slugs.size(); ++ i)
		total += Max(slugs[i]->GetHitPoints(), 0);

	return total;

}

bool Team::HasAliveSlugs() const
{

	for (unsigned int i = 0; i < slugs.size(); ++ i)
	{

		if (slugs[i]->GetHitPoints() > 0)
			return true;

	}

	return false;

}