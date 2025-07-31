#include "Game.h"
#include "Level/GameLevel.h"

Game::Game()
{
	AddLevel(new GameLevel());
}