#pragma once

#include "player.h"

/*
	class NetworkPlayer
	A player controlled by a another user (or by another user computer)
*/

class NetworkPlayer : public Player
{

protected:

public:

	NetworkPlayer(const std::string& playerName);

};