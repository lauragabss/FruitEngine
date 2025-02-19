#include <iostream>
#include "Game.h"

int main(int argc, char* argv[]) {

	Game game;

	game.Initialize();

	game.Setup();

	game.Run();

	game.Destroy();

	return 0;
}
