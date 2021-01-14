// The actual input handling
#include "input.h"
#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

// utf8::unchecked::distance(str.begin(), str.end()) - leo

namespace input {
    bool taking_input;
    int current_scroll;
    bool selecting;
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
            temp.highest_position = 0;
            cursors.push_back(temp);
        }

        taking_input = true;
        SDL_StartTextInput();
    }

    void Logic() {
        if (!taking_input) return;
    }

    void InsertText(std::string text) {
        for (int i = 0; i < (int) cursors.size(); i++) { // For all cursors...
            std::stringstream string_stream(text); // Create a stringstream for the text we're inserting
            std::string output;                    // We'll need the output later
            while (std::getline(string_stream, output)) { // Iterate through all lines,
                output.erase(std::remove(output.begin(), output.end(), '\r'), output.end()); // Strip \r... dammit Windows.
                (*current_text)[cursors[i].line].insert(cursors[i].position, output); // Insert the current line of text into our text
                cursors[i].position += output.length(); // Update cursor position
                if (!string_stream.eof()) { // If we haven't hit the end of the file,
                    InsertNewline(); // Insert a newline
                }
            }
            cursors[i].highest_position = cursors[i].position; // Update highest_position

        }
    }

    void InsertNewline() { // The user pressed enter!
        for (int i = 0; i < (int)cursors.size(); i++) { // For all cursors...
            std::string first_part  = (*current_text)[cursors[i].line].substr(0, cursors[i].position);
            std::string second_part = (*current_text)[cursors[i].line].substr(cursors[i].position, (*current_text)[cursors[i].line].length());
            (*current_text)[cursors[i].line] = first_part;
            (*current_text).insert((*current_text).begin() + cursors[i].line + 1,second_part);
            cursors[i].line++;
            cursors[i].position = 0;
            cursors[i].highest_position = 0;
        }
    }

    void SelectAll() {
        selecting = true;
        cursors.resize(1);
        cursors[0].started_selection_position = 0;
        cursors[0].started_selection_line = 0;

        cursors[0].line = (*current_text).size() - 1;
        cursors[0].position = (*current_text)[cursors[0].line].size();
    }

    int CheckIfShiftHeld() {
        if (SDL_GetModState() & KMOD_SHIFT) {
            if (!selecting) {
                for (int i = 0; i < (int)cursors.size(); i++) { // For all cursors...
                    cursors[i].started_selection_position = cursors[i].position;
                    cursors[i].started_selection_line = cursors[i].line;
                }
                selecting = true;
            }
        }
        else {
            if (selecting) {
                selecting = false;
                return true;
            }
        }
        return false;
    }

    void MoveCursorUp() {
        bool reset = CheckIfShiftHeld();  // Only returns true if you don't hold shift
        cursors.resize(1);
        if (reset && (cursors[0].line > cursors[0].started_selection_line)) {
            cursors[0].line = cursors[0].started_selection_line;
        }
        if (cursors[0].line > 0) cursors[0].line--;
        cursors[0].position = cursors[0].highest_position;
        if (cursors[0].position > (*current_text)[cursors[0].line].length()) cursors[0].position = (*current_text)[cursors[0].line].length();
    }

    void MoveCursorDown() {
        bool reset = CheckIfShiftHeld();  // Only returns true if you don't hold shift
        cursors.resize(1);
        if (reset && (cursors[0].line < cursors[0].started_selection_line)) {
            cursors[0].line = cursors[0].started_selection_line;
        }
        if (cursors[0].line < (*current_text).size() - 1) cursors[0].line++;
        cursors[0].position = cursors[0].highest_position;
        if (cursors[0].position > (*current_text)[cursors[0].line].length()) cursors[0].position = (*current_text)[cursors[0].line].length();
    }

    void MoveCursorLeft() {
        bool reset = CheckIfShiftHeld();  // Only returns true if you don't hold shift
        cursors.resize(1);
        if (reset) {
            if (cursors[0].position > cursors[0].started_selection_position) {
                cursors[0].position = cursors[0].started_selection_position;
                cursors[0].highest_position = cursors[0].position;
            }
            return;
        }
        if (cursors[0].position > 0) {
            cursors[0].position--;
            cursors[0].highest_position = cursors[0].position;
        } else {
            if (cursors[0].line > 0) {
                cursors[0].line--;
                cursors[0].position = (*current_text)[cursors[0].line].length();
                cursors[0].highest_position = cursors[0].position;
            }
        }
    }

    void MoveCursorRight() {
        bool reset = CheckIfShiftHeld();  // Only returns true if you don't hold shift
        cursors.resize(1);
        if (reset) {
            if (cursors[0].position < cursors[0].started_selection_position) {
                cursors[0].position = cursors[0].started_selection_position;
                cursors[0].highest_position = cursors[0].position;
            }
            return;
        }
        if (cursors[0].position < (*current_text)[cursors[0].line].length()) {
            cursors[0].position++;
            cursors[0].highest_position = cursors[0].position;
        } else {
            if (cursors[0].line < (*current_text).size() - 1) {
                cursors[0].line++;
                cursors[0].position = 0;
                cursors[0].highest_position = 0;
            }
        }
    }

    std::string GetSelectedText() {
        rect positions = ReorderSelectionPositions();

        if (positions.y == positions.y2) {
            return (*current_text)[positions.y].substr(positions.x, positions.x2 - positions.x);
        }

        std::string return_string;
        return_string += (*current_text)[positions.y].substr(positions.x, std::string::npos); // Get the selected part of the first line
        return_string += "\n";
        for (int j = positions.y + 1; j < positions.y2; j++) {
            return_string += (*current_text)[j] + "\n";
        }

        return_string += (*current_text)[positions.y2].substr(0, positions.x2); // Get the selected part of the last line

        return return_string;
    }

    void RemoveCharacters(int x, int y, int x2, int y2) {
        std::string rest_of_string = (*current_text)[y2].substr(x2,std::string::npos); // Get the rest of the last line
        for (int j = y2; j > y; j--) {
            (*current_text).erase((*current_text).begin() + j); // Remove the current line
        }
        (*current_text)[y].erase(x, std::string::npos);
        (*current_text)[y] += rest_of_string;
    }

    rect ReorderSelectionPositions() {
        rect positions;
        bool in_front = false;
        if (input::cursors[0].line > input::cursors[0].started_selection_line) {
            in_front = true;
        }
        else if (input::cursors[0].line == input::cursors[0].started_selection_line) {
            if (input::cursors[0].position >= input::cursors[0].started_selection_position) {
                in_front = true;
            }
        }

        if (in_front) {
            positions.x = cursors[0].started_selection_position;
            positions.x2 = cursors[0].position;
            positions.y = cursors[0].started_selection_line;
            positions.y2 = cursors[0].line;
        }
        else {
            positions.x = cursors[0].position;
            positions.x2 = cursors[0].started_selection_position;
            positions.y = cursors[0].line;
            positions.y2 = cursors[0].started_selection_line;
        }
        return positions;
    }

    void RemoveSelectionCharacters() {
        rect positions = ReorderSelectionPositions();
        cursors[0].position = positions.x;
        cursors[0].highest_position = positions.x;
        cursors[0].line = positions.y;
        RemoveCharacters(positions.x, positions.y, positions.x2, positions.y2);
        selecting = false;
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
                cursors[i].highest_position = current_size;

            } else {
                (*current_text)[cursors[i].line].erase(cursors[i].position - 1, 1); // Delete the character before the cursor
                cursors[i].position--; // Move the cursor back one
                cursors[i].highest_position = cursors[i].position;
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
                if (selecting) {
                    RemoveSelectionCharacters();
                }
                else {
                    Backspace();
                }
            }
            else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
            {
                char* heck = SDL_GetClipboardText();
                InsertText(heck);
                SDL_free(heck);
            }
            else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
            {
                SDL_SetClipboardText(GetSelectedText().c_str());
            }
            else if (e.key.keysym.sym == SDLK_x && SDL_GetModState() & KMOD_CTRL)
            {
                SDL_SetClipboardText(GetSelectedText().c_str());
                RemoveSelectionCharacters();
            }
            else if (e.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL) {
                SelectAll();
            }
            else if (e.key.keysym.sym == SDLK_RETURN) {
                InsertNewline();
            }
            else if (e.key.keysym.sym == SDLK_UP) {
                MoveCursorUp();
            }
            else if (e.key.keysym.sym == SDLK_DOWN) {
                MoveCursorDown();
            }
            else if (e.key.keysym.sym == SDLK_LEFT) {
                MoveCursorLeft();
            }
            else if (e.key.keysym.sym == SDLK_RIGHT) {
                MoveCursorRight();
            }
        }
        //Special text input event
        else if (e.type == SDL_TEXTINPUT)
        {
            //Append character(s)
            if (selecting) {
                RemoveSelectionCharacters();
            }
            InsertText(e.text.text);
        }
    }
}