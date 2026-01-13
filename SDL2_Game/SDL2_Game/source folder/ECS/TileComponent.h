#pragma once
// --- MODIFICATION LOG (Tile Component) ---
// 1. Code Quality: Added member initializer lists to all constructors to fix C26495 warnings.
// 2. Type Safety: Implemented static_cast<float> when adding the TransformComponent 
//    to resolve C4244 warnings.

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	TransformComponent* transform; // Pointer to the position/size data.
	SpriteComponent* sprite;       // Pointer to the visual texture data.

	SDL_Rect tileRect; // The rectangle defining the tile's position/size in the map.
	int tileID;        // The type identifier (0 for floor, 1 for wall).
	const char* path;  // The file path to the texture image.


	// Default Constructor (Initializes pointers and primitives safely)
	TileComponent() : transform(nullptr), sprite(nullptr), tileID(0), path(nullptr) {}

	// Parameterized Constructor
	TileComponent(int x, int y, int w, int h, int id)
		: transform(nullptr), sprite(nullptr), tileID(id), path(nullptr) // Initialize members via list
	{
		// Set the tile's initial coordinates and dimensions
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		// Determine the texture path based on the Tile ID
		switch (tileID)
		{
		case 0: // Floor tile
			path = "assets/tileMap/floor.png";
			break;
		case 1: // Wall tile (Solid/Collidable)
			path = "assets/tileMap/wall.png";
			break;
		default:
			break;
		}
	}

	void init() override
	{
		// Add the Position Component 
		//
		entity->addComponent<TransformComponent>(static_cast<float>(tileRect.x),
			static_cast<float>(tileRect.y),
			tileRect.w,
			tileRect.h,
			1); // Scale of 1
		transform = &entity->getComponent<TransformComponent>();

		// Add the Visual Component (loads the determined texture)
		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}

};