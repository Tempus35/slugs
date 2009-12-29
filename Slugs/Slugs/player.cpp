#include "player.h"
#include "game.h"

Player::Player(const std::string& playerName, PlayerType playerType)
{

	type = playerType;

	name = playerName;

	activeSlug = NULL;
	activeTeam = NULL;

	timeRemaining = 0.0f;

}

const std::string& Player::GetName() const
{

	return name;

}

PlayerType Player::GetType() const
{

	return type;

}

Slug* Player::GetCurrentSlug() const
{

	return activeSlug;

}

void Player::Update(float elapsedTime)
{

	if (!turnEnding)
	{

		if (timeRemaining != Math::INFINITY)
		{

			timeRemaining -= elapsedTime;

			// End our turn if we ran out of time
			if (timeRemaining <= 0.0f)
				turnEnding = true;

		}

		// End the turn if we killed our slug
		if (!activeSlug->IsAlive())
			turnEnding = true;

	}
	else
	{

		// The turn end timer doesn't run out until nothing is happening in the world
		if (Game::Get()->EnsureNoAction())
			turnEndTimer -= elapsedTime;

		if (turnEndTimer <= 0.0f)
			Game::Get()->EndTurn();

	}

	// Render debugging info
	if (Game::Get()->GetGameBool(GameBool_Debug))
		DebugRender();

}

void Player::TurnBegins()
{

	// Reset the turn state
	ResetForTurn();

	// Select the next slug in the chain if we aren't allowed to choose our own
	if ((!Game::Get()->GetGameBool(GameBool_CanChooseSlug)) || (activeSlug == NULL))
		SelectNextSlug();

}

void Player::ResetForTurn()
{

	timeRemaining = Game::Get()->GetGameFloat(GameFloat_TurnTime);
	shotsRemaining = Game::Get()->GetGameInt(GameInt_ShotsPerTurn);
	acted = false;

	turnEnding = false;
	turnEndTimer = Game::Get()->GetGameFloat(GameFloat_TurnEndTime);

}

void Player::TurnEnds()
{

	// Stop whatever the active slug is doing
	activeSlug->StopEverything();

}

void Player::SlugFired()
{

	shotsRemaining --;

	// End the turn if we are out of shots
	if (shotsRemaining <= 0)
		turnEnding = true;

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

	Slug* slug = NULL;

	if (activeSlug == NULL)
	{

		// This should only be hit to set the initial slug on the players first turn
		slug = availableSlugs[0];

	}
	else
	{

		//
		// Find the next slug in the chain
		//

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

	}

	// If we didn't find another slug, we must be the last one left on our team
	if (slug != NULL)
	{

		activeSlug = slug;
		activeTeam = slug->GetTeam();

	}

	MoveCameraToActiveSlug();

}

void Player::MoveCameraToActiveSlug()
{

	Game::Get()->GetCamera()->MoveTo(activeSlug, false);

}

float Player::GetTurnTimeRemaining() const
{

	return timeRemaining;

}

bool Player::HasActed() const
{

	return acted;

}

bool Player::IsTurnEnding() const
{

	return turnEnding;

}

void Player::EndTurn()
{

	turnEnding = true;

}

void Player::DebugRender()
{

	if (activeSlug)
	{
		
		if ((timeRemaining >= 0.0f) && (!turnEnding))
			Renderer::Get()->DrawDebugCircle(activeSlug->GetPosition(), 25.0f, Color(0, 255, 0));
		else if (turnEndTimer >= 0.0f)
			Renderer::Get()->DrawDebugCircle(activeSlug->GetPosition(), 25.0f, Color(255, 255, 0));

	}

}

unsigned int Player::GetNumTeams() const
{

	return teams.size();

}

Team* Player::GetTeam(int index) const
{

	return teams[index];

}

bool Player::HasAliveSlugs() const
{

	for (unsigned int i = 0; i < teams.size(); ++ i)
	{

		if (teams[i]->HasAliveSlugs())
			return true;

	}

	return false;

}