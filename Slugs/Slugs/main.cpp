#include "cSlugs.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR test, int)
{
	cSlugs game;

	if (game.bInit()) {
		game.bUpdate();
	}

	return 0;
}
