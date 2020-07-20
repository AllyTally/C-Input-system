/*
            INPUT   
          Ally 2020

    The goal of my content
    is always to entertain
*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h> // Standard IO
#include <cstdlib> // Standard Library
#include <iostream>

#include "graphics.h"
#include "input.h"
#include "editor.h"

// Screen dimension constants
const int   SCREEN_WIDTH = 320;
const int   SCREEN_HEIGHT = 240;
const int   SCREEN_SCALE = 2;
const char* WINDOW_TITLE = "Input Test Window";

// The window we'll be rendering to
SDL_Window* g_window = NULL;
SDL_Surface* g_surface = NULL;
SDL_Renderer* g_renderer = NULL;

int main(int argc, char* args[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create the window
    g_window = SDL_CreateWindow(
        WINDOW_TITLE, // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        SCREEN_WIDTH, // width, in pixels
        SCREEN_HEIGHT, // height, in pixels
        SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE // flags - see below
    );
    // And the renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (g_window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    // Set up the renderer
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
    // Resize the window
    SDL_RenderSetLogicalSize(g_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderSetIntegerScale(g_renderer, SDL_FALSE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetWindowSize(g_window, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE);

    SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Show the window

    SDL_ShowWindow(g_window);

    // Get the window's surface
    g_surface = SDL_GetWindowSurface(g_window);

    // Make it black!
    SDL_FillRect(g_surface, NULL, SDL_MapRGB(g_surface->format, 0x00, 0x00, 0x00));

    input::Init();
    graphics::Init();

    input::StartInput(&editor::script_lines);

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            // Input
            input::HandleEvents(e);
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }
        if (quit) break;

        // Logic state machine!
        input::Logic(); // Doesn't do anything if not currently taking input

        //Clear screen
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);

        // Rendering state machine!
        editor::Render();
        //graphics::DrawText(graphics::font, 20, 20, "pogger", 255, 0, 255, 255);

        //Update screen
        SDL_RenderPresent(g_renderer);
    }
    SDL_FreeSurface(g_surface);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_surface = NULL;
    g_renderer = NULL;
    g_window = NULL;

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
