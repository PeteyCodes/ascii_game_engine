
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

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *image = IMG_Load("assets/font10x16.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    
    console_image_t *img = console_image_from_rexfile("./assets/cat.xp");
    
    SDL_Event event;
    uint32_t timePerFrame = 1000 / FPS_LIMIT;
    uint32_t frameStart = 0;
    while (1) {
        frameStart = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);
        for (uint32_t y = 0; y < img->height; y++) {
            for (uint32_t x = 0; x < img->width; x++) {
                console_cell_t *cell = console_image_cell(img, x, y);
                int tex_x = (cell->glyph % 16) * 10;
                int tex_y = (cell->glyph / 16) * 16;
                SDL_Rect src_rect = {tex_x, tex_y,  10, 16};
                SDL_Rect dst_rect = {x*10, y*16, 10, 16};

                SDL_SetTextureColorMod(texture, RED(cell->fg_color), GREEN(cell->fg_color), BLUE(cell->fg_color));
                SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
            }
        }
        SDL_RenderPresent(renderer);

        // Limit our top FPS
        int32_t sleepTime = timePerFrame - (SDL_GetTicks() - frameStart);
        if (sleepTime > 0) {
            SDL_Delay(sleepTime);
        }
    }

    SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    IMG_Quit();
	SDL_Quit();

	return 0;

}


