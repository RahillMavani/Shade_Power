//INCLUDE ALL THE HEADERS AND SDL
#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <vector>
#include <SDL_mixer.h>

//this class exists lol (defining later).
class ColliderComponent;

class Vector2D; // Forward declaration

class GameLoop
{
public:
	GameLoop(); //constructor: runs when it is first created
	~GameLoop(); // decontructor: runs when it says "delete this" (basically cleans up memory).

	// Opens the window. Takes arguments for Title, X/Y position, Size, and Mode.
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);

	// This handles the input events.
	void handleEvents();

	//Runs every frame calculating positions,etc.
	void update();

	//Renders out everything
	void render();

	//reset function
	void resetGame();

	//health potions
	void spawnPotion(float x, float y);

	//Shuts down SDL and closes the window.
	void clean();

	//A bool getter to know whether the game is still running.
	bool running() { return isRunning; }

	//enemy spawn function
	void spawnEnemy(float x, float y);

	//bullet spawn function
	static void spawnProjectile(Vector2D pos, Vector2D vel);

	//blood particle
	void spawnBlood(float x, float y);

	//only one addTile which add the tiles with their ids, and positions.
	static void addTile(int id, int x, int y);

	//Renderer to render.
	static SDL_Renderer* renderer;
	//Stores input values and keys.
	static SDL_Event event;

	
	//Store how many seconds has passed since last frame (to make my movement smoother).
	static float delta_time;
	//At what time the first frame started.
	static Uint32 frame_start;


private:
	//whether the game is running or not.
	bool isRunning;
	//window of the program.
	SDL_Window* window;

};