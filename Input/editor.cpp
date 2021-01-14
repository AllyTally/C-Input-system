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
        //Draw cursors
        for (int i = 0; i < (int)input::cursors.size(); i++) {
            if (input::selecting) {
                int x = input::cursors[i].started_selection_position;
                int y = input::cursors[i].started_selection_line;
                int w = input::cursors[i].position - x;
                int h = input::cursors[i].line - y;
                if (input::cursors[i].line == input::cursors[i].started_selection_line) {
                    graphics::DrawRectangle(16 + x * 8, 20 + y * 8, w * 8, (h * 8) + 8, 0, 127, 255);
                }
                else {
                    bool in_front = false;
                    if (input::cursors[i].line > input::cursors[i].started_selection_line) {
                        in_front = true;
                    }
                    else if (input::cursors[i].line == input::cursors[i].started_selection_line) {
                        if (input::cursors[i].position >= input::cursors[i].started_selection_position) {
                            in_front = true;
                        }
                    }


                    if (in_front) {
                        graphics::DrawRectangle(16 + x * 8, 20 + y * 8, (script_lines[input::cursors[i].started_selection_line].size() - x) * 8, 8, 0, 127, 255);
                        for (int j = 1; j < h; j++) {
                            graphics::DrawRectangle(16, 20 + (y + j) * 8, (script_lines[y + j].size() * 8), 8, 0, 127, 255);
                        }
                        graphics::DrawRectangle(16, 20 + input::cursors[i].line * 8, input::cursors[i].position * 8, 8, 0, 127, 255);
                    }
                    else {
                        graphics::DrawRectangle(16, 20 + y * 8, input::cursors[i].started_selection_position * 8, 8, 0, 127, 255);
                        for (int j = 1; j < input::cursors[i].started_selection_line - input::cursors[i].line; j++) {
                            graphics::DrawRectangle(16, 20 + (input::cursors[i].line + j) * 8, (script_lines[input::cursors[i].line + j].size() * 8), 8, 0, 127, 255);
                        }
                        graphics::DrawRectangle(16 + input::cursors[i].position * 8, 20 + input::cursors[i].line * 8, (script_lines[input::cursors[i].line].size() - input::cursors[i].position) * 8, 8, 0, 127, 255);
                    }
                }
            }
            graphics::DrawText(graphics::font, 16 + (input::cursors[i].position * 8), 20 + (input::cursors[i].line * 8), "_", 255, 0, 255, false);
        }

        //Draw text
        for (int i = 0; i < 25; i++)
        {
            if (i + current_scroll < (int)script_lines.size())
            {
                graphics::DrawText(graphics::font, 16, 20 + (i * 8), script_lines[i], 255, 0, 255, false);
            }
        }
    }
}