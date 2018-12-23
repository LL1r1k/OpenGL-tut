#include "Game.h"

int main(int args, char** argv)
{
	Game game("OpenGL tut");
		
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();		
	}
	return 0;
}
