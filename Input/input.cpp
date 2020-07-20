// The actual input handling
#include "input.h"
#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>
#include <vector>


namespace input {
    bool taking_input;
    int current_scroll;
    std::vector<std::string>* current_text;
    std::vector<cursor> cursors;

    void Init() {
        taking_input = false;
        current_scroll = 0;
    }

    void StartInput(std::vector<std::string> *text) {
        current_text = text;
        if (current_text->size() == 0) {
            current_text->push_back("");
        }

        taking_input = true;
        SDL_StartTextInput();
    }

    void Logic() {
        if (!taking_input) return;
    }

    void HandleEvents(SDL_Event e) {
        if (!taking_input) return;

        if (e.type == SDL_KEYDOWN) {
            // Handle backspace
            if (e.key.keysym.sym == SDLK_BACKSPACE)
            {
                // Remove the character
                current_text[0].pop_back();
            }
            else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
            {
                (*current_text)[0] = SDL_GetClipboardText();
            }
        }
    }
}