#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <cstdlib>
#include "InfoNES/InfoNES_System.h"
#include "InfoNES/InfoNES.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Event event;
bool quit;


void drawRandomPixels() {
    while (SDL_PollEvent(&event))
    {
        // window close event
        if (event.type == SDL_QUIT)
        {
            quit = true;
            break;
        }
    } // end poll events


    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    auto * pixels = static_cast<Uint8 *>(surface->pixels);

    for (int i=0; i < NES_DISP_HEIGHT*NES_DISP_HEIGHT; i++) {

        pixels[i*2] = 0b11100000;
        pixels[i*2+1] = 0b00000111;

    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(screenTexture);
}

void sdl_simple_init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(NES_DISP_WIDTH, NES_DISP_HEIGHT, 0, &window, &renderer);

    //flag 和 depth 实际上没用，并且它们会在sdl3中被删除
    surface = SDL_CreateRGBSurfaceWithFormat(0, NES_DISP_WIDTH, NES_DISP_HEIGHT, 0, SDL_PIXELFORMAT_RGB565);
    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    sdl_simple_init();
    InfoNES_Load("nullptr");
    InfoNES_Main();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(d, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    // repeatedly calling mainloop on desktop
    while(!quit) drawRandomPixels();
#endif
}