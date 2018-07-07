
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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


typedef struct {
    uint32_t glyph;
    uint32_t fg_color;
    uint32_t bg_color;
} console_cell_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    console_cell_t *tiles;
} console_image_t;


int main() {

    rex_tile_map_t *map = rex_load_tile_map("./assets/cat.xp");

    if (map != NULL) {
        printf("width: %d, height: %d, layers: %d\n", map->width, map->height, map->layer_count);

        for (uint32_t l = 0; l < map->layer_count; l++) {
            printf("Layer %d\n", l);
            rex_tile_layer_t *layer = &map->layers[l];
            uint32_t tile_count = layer->width * layer->height;
            for (uint32_t t = 0; t < tile_count; t++) {
                rex_tile_t *tile = &layer->tiles[t];
                printf("%d (%d,%d,%d) (%d,%d,%d)\n", tile->char_code, tile->fg_red, tile->fg_green, tile->fg_blue, tile->bg_red, tile->bg_green, tile->bg_blue);
            }

        }
    }


    /*
    time_t t;
    srand((unsigned) time(&t));

	SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

	SDL_Window *window = SDL_CreateWindow("Graphix Test",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_FULLSCREEN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *image = IMG_Load("assets/font10x16.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

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
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                int idx = 35;//rand() % 256;
                Uint8 r = rand() % 256;
                Uint8 g = rand() % 256;
                Uint8 b = rand() % 256;
                int x = (idx % 16) * 10;
                int y = (idx / 16) * 16;
                SDL_Rect src_rect = {x, y,  10, 16};
                SDL_Rect dst_rect = {col*10, row*16, 10, 16};
                SDL_SetTextureColorMod(texture, r, g, b);
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
*/
	return 0;


}


