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
        if (cursors.size() == 0) {
            cursor temp;
            temp.position = 0;
            temp.line = 0;
            cursors.push_back(temp);
        }

        taking_input = true;
        SDL_StartTextInput();
    }

    void Logic() {
        if (!taking_input) return;
    }

    void InsertText(std::string text) {
        for (int i = 0; i < (int) cursors.size(); i++) {
            (*current_text)[cursors[i].line].insert(cursors[i].position,text);
            cursors[i].position += text.length();
        }
    }

    void Backspace() { // The user pressed backspace!
        for (int i = 0; i < (int)cursors.size(); i++) { // For all cursors...
            if (cursors[i].position == 0) { // If we're right at the start of the newline...
                if (cursors[i].line == 0) return; // Don't do anything if it's at the start of the file.
                // Great! Now we need to merge the two lines...
                int current_size = (*current_text)[cursors[i].line - 1].length(); // Save the length of the previous line
                (*current_text)[cursors[i].line - 1] += (*current_text)[cursors[i].line]; // Add the current line to the previous line
                (*current_text).erase((*current_text).begin() + cursors[i].line); // Remove the current line

                cursors[i].line--; // Make the cursor go back a line
                cursors[i].position = current_size; // Position the cursor at the end of the previous (now current) line before the new text was added

            } else {
                (*current_text)[cursors[i].line].erase(cursors[i].position - 1); // Delete the character before the cursor
                cursors[i].position--; // Move the cursor back one
            }
        }
    }

    void HandleEvents(SDL_Event e) {
        if (!taking_input) return;

        if (e.type == SDL_KEYDOWN) {
            // Handle backspace
            if (e.key.keysym.sym == SDLK_BACKSPACE)
            {
                // Remove the character
                Backspace();
            }
            else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
            {
                InsertText(SDL_GetClipboardText());
            }
        }
        //Special text input event
        else if (e.type == SDL_TEXTINPUT)
        {
            //Not copy or pasting
            if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
            {
                //Append character(s)
                InsertText(e.text.text);
            }
        }
    }
}