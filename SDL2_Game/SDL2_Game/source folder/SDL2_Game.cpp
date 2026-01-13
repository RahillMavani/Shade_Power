//INCLUDING ALL THE HEADERS
#define SDL_MAIN_HANDLED 
#include <SDL.h>           
#include <iostream>         
#include "Header files/GameLoop.h" 

//global variable
GameLoop* gameLoop = nullptr; 

//Starting the game with main function, takes argument counts and stores em in an array.
int main(int argc, char* argv[])
{

	//creating a new gameloop object
	gameLoop = new GameLoop(); 
	//setting up the window
	gameLoop->init("SDL2_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1120, 960, false);

	//while game is running, do all these stuff.
	while (gameLoop->running()) 
	{

		
		gameLoop->handleEvents();
		
		gameLoop->update();
		
		gameLoop->render();


	}
	//when game ends, clean the resources.
	gameLoop->clean(); 
	//deletes the gameloop
	delete gameLoop;   

	// 0 means ended succesfully
	return 0; 
}