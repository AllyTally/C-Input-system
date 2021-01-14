#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h> // SDL
#include <SDL_image.h> // SDL_Image
#include <SDL_ttf.h> // SDL_ttf
#include <string>

extern SDL_Window* g_window;
extern SDL_Surface* g_surface;
extern SDL_Renderer* g_renderer;

namespace graphics
{
    extern TTF_Font* font;
    void Init();

    bool DrawText(TTF_Font* font, int x, int y, std::string text, int r, int g, int b, int a);
    bool DrawText(TTF_Font* font, int x, int y, std::string text, int r, int g, int b);
    void DrawTextBordered(TTF_Font* font, int x, int y, std::string text, int r, int g, int b, int a);
    void DrawTextBordered(TTF_Font* font, int x, int y, std::string text, int r, int g, int b);
    void DrawRectangle(int x, int y, int w, int h, int r, int g, int b, int a);
    void DrawRectangle(int x, int y, int w, int h, int r, int g, int b);
    void DrawRectangleOutline(int x, int y, int w, int h, int r, int g, int b, int a);
    void DrawRectangleOutline(int x, int y, int w, int h, int r, int g, int b);
}

#endif