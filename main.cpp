#include <iostream>
#include "Game.h"

int main()
	{
	//init srand
	std::srand(static_cast<unsigned>(time(NULL)));
	//Init game engine 
	Game game; 


	//Game loop
	while (game.getWindowIsOpen() && !game.getEndGame())
	{
		
		//Update
		game.update();
		//Render
		game.render();
	}
	return 0;	
}