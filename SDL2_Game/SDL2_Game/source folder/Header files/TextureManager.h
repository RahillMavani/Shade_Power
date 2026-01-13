#pragma once
#include "GameLoop.h"
#include "TileMap.h"

class TextureManager
{

public:
	//basically takes the file path to load a texture
	static SDL_Texture* LoadTexture(const char* fileName);
	//draws the texture at a spot.
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);

};