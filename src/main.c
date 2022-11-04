#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "InfoNES/InfoNES_System.h"
#include "InfoNES/InfoNES.h"
#include "stdbool.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Event e;
SDL_Texture *texture;
bool quit;

extern DWORD dwPad1;
extern DWORD dwPad2;
extern DWORD dwSystem;

void loop() {
    while (SDL_PollEvent(&e)) {
        // window close event
        if (e.type == SDL_QUIT) {
            quit = true;
            break;
        }

        switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_d:
                        dwPad1 |= (1 << 7);
                        break;
                    case SDLK_a:
                        dwPad1 |= (1 << 6);
                        break;
                    case SDLK_s:
                        dwPad1 |= (1 << 5);
                        break;
                    case SDLK_w:
                        dwPad1 |= (1 << 4);
                        break;
                    case SDLK_1:
                        dwPad1 |= (1 << 3);
                        break;    /* Start */
                    case SDLK_2:
                        dwPad1 |= (1 << 2);
                        break;    /* Select */
                    case SDLK_j:
                        dwPad1 |= (1 << 1);
                        break;    /* 'B' */
                    case SDLK_k:
                        dwPad1 |= (1 << 0);
                        break;    /* 'A' */
                } /* keydown */
                break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_d:
                        dwPad1 &= ~(1 << 7);
                        break;
                    case SDLK_a:
                        dwPad1 &= ~(1 << 6);
                        break;
                    case SDLK_s:
                        dwPad1 &= ~(1 << 5);
                        break;
                    case SDLK_w:
                        dwPad1 &= ~(1 << 4);
                        break;
                    case SDLK_1:
                        dwPad1 &= ~(1 << 3);
                        break;   /* Start */
                    case SDLK_2:
                        dwPad1 &= ~(1 << 2);
                        break;   /* Select */
                    case SDLK_j:
                        dwPad1 &= ~(1 << 1);
                        break;   /* 'B' */
                    case SDLK_k:
                        dwPad1 &= ~(1 << 0);
                        break;   /* 'A' */
                } /* keyup */
                break;
        }
    } // end poll events


    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *) (surface->pixels);

    /*
     * InfoNES
     */
    InfoNES_Cycle();
    memcpy(pixels, WorkFrame, NES_DISP_WIDTH * NES_DISP_HEIGHT * 2);

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_simple_init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_CreateWindowAndRenderer(NES_DISP_WIDTH*3, NES_DISP_HEIGHT*3, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED , &window, &renderer);

    //flag 和 depth 实际上没用，并且它们会在sdl3中被删除
    surface = SDL_CreateRGBSurfaceWithFormat(0, NES_DISP_WIDTH, NES_DISP_HEIGHT, 0, SDL_PIXELFORMAT_RGB565);
    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
        exit(1);
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, NES_DISP_WIDTH,
                                NES_DISP_HEIGHT);
}

int main(int argc, char *argv[]) {
    sdl_simple_init();
    InfoNES_Load(NULL);

    InfoNES_Init();


#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    // repeatedly calling mainloop on desktop
    while (!quit) {
        loop();
        SDL_Delay(16);
    };
#endif
    InfoNES_Fin();
}