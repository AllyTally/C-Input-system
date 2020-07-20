#pragma once

#ifndef EDITOR_H
#define EDITOR_H

#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>
#include <vector>

namespace editor {
    extern std::vector<std::string> script_lines;
    extern int current_scroll;

    void Render();
}

#endif