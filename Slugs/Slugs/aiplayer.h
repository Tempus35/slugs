#pragma once

#include "player.h"

/*
	class AIPlayer
	A player controlled by AI
*/

class AIPlayer : public Player
{

protected:

public:

	// Fired when the players turn begins
	virtual void TurnBegins();

};