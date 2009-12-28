#pragma once

#include "player.h"

/*
	class AIPlayer
	A player controlled by computer AI
*/

class AIPlayer : public Player
{

protected:

public:

	// Constructor
	AIPlayer(const std::string& playerName);

	// Fired when the players turn begins
	virtual void TurnBegins();

};