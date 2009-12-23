#include "player.h"
#include "game.h"

void Player::TurnBegins()
{

	if (!Game::Get()->IsFlagSet(GameFlag_CanChooseSlug))
		SelectNextSlug();

	MoveCameraToActiveSlug();

}

void Player::AddTeam(Team* team)
{

	ASSERT(team != NULL);

	teams.push_back(team);

}

void Player::PlaceInWorld()
{

	for (unsigned int i = 0; i < teams.size(); ++ i)
		teams[i]->PlaceInWorld();

}

void Player::SelectNextSlug()
{

	//
	// Get slugs from our teams
	//
	
	std::vector<Slug*> availableSlugs;
	
	for (unsigned int i = 0; i < teams.size(); ++ i)
		teams[i]->GetSlugs(availableSlugs, false);

	//
	// Find the next slug in the chain
	//

	Slug* slug = NULL;
	bool get = false;
	int lastSlugIndex = -1;
	for (unsigned int i = 0; i < availableSlugs.size(); ++ i)
	{

		if ((get) && (availableSlugs[i]->GetHitPoints() > 0))
		{

			slug = availableSlugs[i];
			break;

		}

		if (availableSlugs[i] == activeSlug)
		{

			lastSlugIndex = i;
			get = true;

		}

	}

	if (!slug)
	{

		ASSERT(lastSlugIndex != -1);

		for (int i = 0; i < lastSlugIndex; ++ i)
		{

			if (availableSlugs[i]->GetHitPoints() > 0)
			{

				slug = availableSlugs[i];
				break;

			}

		}

	}

	ASSERT(slug != NULL);

	activeSlug = slug;
	activeTeam = slug->GetTeam();

}

void Player::MoveCameraToActiveSlug()
{

	Game::Get()->GetCamera()->MoveTo(activeSlug);

}