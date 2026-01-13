#include "Header files/TileMap.h"
#include "Header files/TextureManager.h"
#include "Header files/GameLoop.h"
#include <fstream> //reading the map file
#include <cstdlib> // ASCII to Integer conversion


// Default Constructor
TileMap::TileMap()
{
	// Currently empty, as map loading is handled by the static LoadMap function.
}

// Destructor
TileMap::~TileMap()
{
}


// Reads a map file and creates tile entities based on the numbers inside.
// path: Location of the text file ("assets/map.map").
// sizeX, sizeY: The dimensions (width and height) of the map grid.
void TileMap::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;

	// 1. Open the map file located at 'path'.
	mapFile.open(path);

	// 2. Loop through the grid dimensions (Rows -> Columns).
	for (int y = 0; y < sizeY; y++) // Outer loop runs for each row
	{
		for (int x = 0; x < sizeX; x++) // Inner loop runs for each column
		{
			// Read one single character (the tile ID) from the file.
			mapFile.get(tile);

			// 3. Create the Tile Entity:
			//Converts the read character ('0', '1', etc.) into its number (0, 1, etc.).
			//Calculates the pixel position based on the grid index (32x32 tiles).
			GameLoop::addTile(atoi(&tile), x * 32, y * 32);

			// 4. Skip the comma
			mapFile.ignore(1);
		}
	}

	// 5. Close the map file to release the resource.
	mapFile.close();
}