#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>
#include <vector>

namespace input {
    struct cursor {
        int position;
        int line;
    };
    extern bool taking_input;
    extern int current_scroll;
    extern std::vector<std::string>* current_text;
    extern std::vector<cursor> cursors;

    void Init();
    void Logic();
    void StartInput(std::vector<std::string>* text);
    void HandleEvents(SDL_Event e);
}

#endif