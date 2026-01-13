#pragma once
#include <string>

class TileMap
{

public:
	// Constructor: Runs when the map is first created.
	TileMap();
	// Destructor: Runs when the map object is deleted (cleans up memory).
	~TileMap();

	// Static function to load the map data from an external file.
	// path: The location of the .map file (e.g., "assets/map.map").
	// sizeX, sizeY: The dimensions of the map grid in tiles (35x35).
	static void LoadMap(std::string path, int sizeX, int sizeY);

private:
};