#pragma once
#include <SDL.h>
#include <SDL_ttf.h> 
#include <string>

class UILabel {
public:
    // Constructor: x, y, text to show, font file name, font size
    UILabel(int xpos, int ypos, std::string text, std::string font, int size);

    // Destructor
    ~UILabel();

    // Change the text later (like score)
    void SetText(std::string text);

    // Draw to screen
    void draw();

private:
    SDL_Rect position;
    SDL_Texture* labelTexture;
    SDL_Color textColor;
    TTF_Font* font;
};



