
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "console.h"
#include "rex_loader.h"

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	768

#define NUM_COLS		128
#define NUM_ROWS 		48

#define FPS_LIMIT       30


// Helper macros for working with pixel colors
#define RED(c) ((c & 0xff000000) >> 24)
#define GREEN(c) ((c & 0x00ff0000) >> 16)
#define BLUE(c) ((c & 0x0000ff00) >> 8)
#define ALPHA(c) (c & 0xff)

#define COLOR_FROM_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)


int main() {

    time_t t;
    srand((unsigned) time(&t));

	SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

	SDL_Window *window = SDL_CreateWindow("Graphix Test",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0); //SDL_WINDOW_FULLSCREEN);

    console_t *console = console_create(window, SCREEN_WIDTH, SCREEN_HEIGHT, NUM_ROWS, NUM_COLS, 255, "assets/font10x16.png");
    console_screen_t *screen = console_screen_create(NUM_COLS, NUM_ROWS, 255);
    
    console_view_t *view = console_view_from_rexfile("./assets/cat.xp");
   
    uint32_t x = 0;
    uint32_t y = 0;
    
    SDL_Event event;
    uint32_t timePerFrame = 1000 / FPS_LIMIT;
    uint32_t frameStart = 0;
    while (1) {
        frameStart = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: x -= 1; break;
                    case SDLK_RIGHT: x += 1; break;
                    case SDLK_UP: y -= 1; break;
                    case SDLK_DOWN: y += 1; break;
                }
            }
        }

        console_clear(console);
        console_screen_clear(screen);

        console_screen_put_view_at(screen, view, x, y);
        console_rect_t rect = {10, 30, 10, 2};
        console_screen_put_text_at(screen, "Welcome to the Core", rect, COLOR_FROM_RGBA(0, 255, 0, 255), 255);
        console_render_screen(console, screen);

        // Limit our top FPS
        int32_t sleepTime = timePerFrame - (SDL_GetTicks() - frameStart);
        if (sleepTime > 0) {
            SDL_Delay(sleepTime);
        }
    }

    console_view_destroy(view);
    console_screen_destroy(screen);
    console_destroy(console);
	SDL_DestroyWindow(window);

    IMG_Quit();
	SDL_Quit();

	return 0;

}


