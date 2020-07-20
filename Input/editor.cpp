// Dummy namespace where input would be used
#include "editor.h"
#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>
#include <vector>

#include "graphics.h"
#include "input.h"

namespace editor {
    std::vector<std::string> script_lines; 
    int current_scroll = 0;

    void Render() {
        //Draw text
        for (int i = 0; i < 25; i++)
        {
            if (i + current_scroll < (int) script_lines.size())
            {
                graphics::DrawText(graphics::font, 16, 20 + (i * 8), script_lines[i], 255, 0, 255, false);
            }
        }
    }
}