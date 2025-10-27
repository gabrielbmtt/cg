#include <SDL3/SDL.h>
#include "pixels.h"

void paintPixel (SDL_Renderer* renderer, int x, int y) {
    SDL_RenderPoint (renderer, x, y);
}

void update (SDL_Renderer* renderer) {
    SDL_RenderPresent (renderer);
}

void setPaintColor (SDL_Renderer* renderer, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor (renderer, r, g, b, a);
}

void setWindowBackground (SDL_Renderer* renderer, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor (renderer, r, g, b, a);
    SDL_RenderClear (renderer);
}

void listenEventQuit (SDL_Window* window) {
    SDL_Event event;
    bool isRunning = true;


    while (isRunning) {
        while (SDL_PollEvent (&event) != 0) {
            if(event.type == SDL_EVENT_QUIT)
                isRunning = false;
        }
    }

    SDL_DestroyWindow (window);
    SDL_Quit ();
}

void initializeSDLAndWindow (
    SDL_Window** window,
    SDL_Renderer** renderer,
    int width,
    int height
) {
    SDL_Init (SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer (
        "CG - Esfera", width, height, 0, window, renderer
    );
}