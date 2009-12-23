#include "aiplayer.h"
#include "game.h"

void AIPlayer::TurnBegins()
{

	//
	// Get a list of alive slugs from each team
	//

	if (Game::Get()->IsFlagSet(GameFlag_CanChooseSlug))
	{

		//
		// We can choose which slug to use for our turn
		// TODO: Pick the best slug based on a strategy
		//

		std::vector<Slug*> availableSlugs;
		
		for (unsigned int i = 0; i < teams.size(); ++ i)
			teams[i]->GetSlugs(availableSlugs, true);

		int pick = Random::RandomInt(0, availableSlugs.size() - 1);

		activeSlug = availableSlugs[pick];
		activeTeam = activeSlug->GetTeam();

	}
	else
		SelectNextSlug();

	MoveCameraToActiveSlug();

}