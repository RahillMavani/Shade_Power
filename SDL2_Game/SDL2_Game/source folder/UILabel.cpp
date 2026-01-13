#include "Header files/UILabel.h"
#include "Header files/GameLoop.h" // We need this to access GameLoop::renderer
#include <iostream>

UILabel::UILabel(int xpos, int ypos, std::string text, std::string fontPath, int size)
{
    // Initialize variables
    position.x = xpos;
    position.y = ypos;
    textColor = { 255, 255, 255, 255 }; // White color
    labelTexture = nullptr;

    // Open the font
    font = TTF_OpenFont(fontPath.c_str(), size);
    if (!font) {
        std::cout << "Failed to load font: " << fontPath << " Error: " << TTF_GetError() << std::endl;
    }

    // Set the initial text (in gameloop.cpp)
    SetText(text);
}

UILabel::~UILabel()
{
    // Clean up memory when this label is deleted
    SDL_DestroyTexture(labelTexture);
    TTF_CloseFont(font);
}

void UILabel::SetText(std::string text)
{
    if (font) {
        // Create Surface
        SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (surf) {
            // Delete old texture if it exists
            if (labelTexture) SDL_DestroyTexture(labelTexture);

            // Create new Texture
            labelTexture = SDL_CreateTextureFromSurface(GameLoop::renderer, surf);

            // Update width/height to match new text size
            position.w = surf->w;
            position.h = surf->h;

            SDL_FreeSurface(surf);
        }
    }
}

void UILabel::draw()
{
    //draw the texture onto the screen at the positions
    if (labelTexture) {
        SDL_RenderCopy(GameLoop::renderer, labelTexture, NULL, &position);
    }
}