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
        int highest_position;
        int started_selection_position;
        int started_selection_line;
    };
    struct rect {
        int x;
        int y;
        int x2;
        int y2;
    };
    extern bool taking_input;
    extern int current_scroll;
    extern bool selecting;
    extern std::vector<std::string>* current_text;
    extern std::vector<cursor> cursors;

    void Init();
    void Logic();
    void StartInput(std::vector<std::string>* text);
    void HandleEvents(SDL_Event e);
    void InsertText(std::string text);
    void InsertNewline();
    int CheckIfShiftHeld();
    void MoveCursorUp();
    void MoveCursorDown();
    void MoveCursorLeft();
    void MoveCursorRight();
    std::string GetSelectedText();
    void RemoveCharacters(int x, int y, int x2, int y2);
    rect ReorderSelectionPositions();
    void RemoveSelectionCharacters();
}

#endif