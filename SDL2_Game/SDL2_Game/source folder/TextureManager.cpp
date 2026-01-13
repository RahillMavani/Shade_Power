#include "Header files/TextureManager.h" 
#include "Header files/GameLoop.h"      
#include <SDL_image.h>                  



//Loads the texture
SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	//Loads the texture to ram
	SDL_Surface* tempSurface = IMG_Load(texture);
	//Taking surface and sending it to the GPU
	SDL_Texture* tex = SDL_CreateTextureFromSurface(GameLoop::renderer, tempSurface);
	//Cleaning up ram
	SDL_FreeSurface(tempSurface);
	//return it to the GPU
	return tex; 
}



//It draws the texture on the screen
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	
	SDL_RenderCopyEx(GameLoop::renderer, tex, &src, &dest, 0.0, NULL, flip);
}