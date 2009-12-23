#include "team.h"
#include "game.h"

Team::Team()
{

	name = "";

	numAlive = 0;
	maxHealth = 0;
	currentHealth = 0;

	activeIndex = -1;

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

		// If we don't have an active slug, make this one active
		if (activeIndex == -1)
			activeIndex = 0;

		return true;

	}

	return false;

}

Slug* Team::Next()
{

	activeIndex ++;

	if (activeIndex == slugs.size())
		activeIndex = 0;

	return slugs[activeIndex];

}

Slug* Team::Previous()
{

	activeIndex --;

	if (activeIndex < 0)
		activeIndex = slugs.size() - 1;

	return slugs[activeIndex];;

}

Slug* Team::First()
{

	activeIndex = 0;

	return slugs[activeIndex];;

}

Slug* Team::Last()
{

	activeIndex = slugs.size() - 1;

	return slugs[activeIndex];;

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
	((TextResource*)ResourceManager::Get()->GetResource("text_teamnames"))->GetRandomLine(name, true);

	TextResource* slugNames = (TextResource*)ResourceManager::Get()->GetResource("text_slugnames");

	for (int i = 0; i < numSlugs; ++ i)
	{

		Slug* slug = new Slug(this);

		std::string n;
		slugNames->GetRandomLine(n, true);
		slug->SetName(n);

		Add(slug);

	}

	slugNames->ClearFlags();

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