#include <iostream>
#include "Game/Game.h"
#include "Level/GameLevel.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Game game;
	game.Run();
}