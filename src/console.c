
#include "console.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "rex_loader.h"


// Internal Functions --

static uint32_t view_cell_index_for_rex_index(const uint32_t rex_idx, const uint32_t width, const uint32_t height);


// External Interface --

/* Console Management */

console_t * console_create(SDL_Window *window, 
        uint32_t width, uint32_t height, 
        uint32_t row_count, uint32_t col_count,
        uint32_t bg_color, const char *font_filename) {

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return NULL;
    }
	SDL_RenderSetLogicalSize(renderer, width, height);

    SDL_Surface *image = IMG_Load(font_filename);
    if (image == NULL) {
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    if (texture == NULL) {
        return NULL;
    }

    // Now that all the SDL stuff has successfully completed, we can
    // assemble our console
    console_t *con = calloc(1, sizeof(console_t));
    con->width = width;
    con->height = height;
    con->row_count = row_count;
    con->col_count = col_count;
    con->cell_width = width / col_count;
    con->cell_height = height / row_count;
    con->bg_color = bg_color;
    con->renderer = renderer;
    con->font_texture = texture;
    
    return con;
}

void console_destroy(console_t *console) {
    SDL_DestroyTexture(console->font_texture);
	SDL_DestroyRenderer(console->renderer);
    free(console);
}

void console_clear(console_t *console) {
    SDL_SetRenderDrawColor(console->renderer, RED(console->bg_color), GREEN(console->bg_color), BLUE(console->bg_color), ALPHA(console->bg_color));
    SDL_RenderClear(console->renderer);
}

void console_render_screen(console_t *console, console_screen_t *screen) {
    // Render all cells on the given screen to the given console
    for (uint32_t y = 0; y < screen->height; y++) {
        for (uint32_t x = 0; x < screen->width; x++) {
            console_cell_t *cell = console_screen_cell(screen, x, y);
            int tex_x = (cell->glyph % 16) * 10;
            int tex_y = (cell->glyph / 16) * 16;
            SDL_Rect src_rect = {tex_x, tex_y,  10, 16};
            SDL_Rect dst_rect = {x*10, y*16, 10, 16};

            SDL_SetTextureColorMod(console->font_texture, RED(cell->fg_color), GREEN(cell->fg_color), BLUE(cell->fg_color));
            SDL_RenderCopy(console->renderer, console->font_texture, &src_rect, &dst_rect);
        }
    }
    SDL_RenderPresent(console->renderer);
}


/* Console Screens */

console_screen_t * console_screen_create(uint32_t width, uint32_t height, uint32_t bg_color) {
    console_cell_t *cells = calloc(width * height, sizeof(console_cell_t));
    if (cells == NULL) {
        return NULL;
    }

    console_screen_t *screen = calloc(1, sizeof(console_screen_t));
    screen->width = width;
    screen->height = height;
    screen->bg_color = bg_color;
    screen->cells = cells;

    return screen;
}

void console_screen_destroy(console_screen_t *screen) {
    free(screen->cells);
    free(screen);
}

void console_screen_clear(console_screen_t *screen) {
    uint32_t cell_count = screen->width * screen->height;
    for (uint32_t idx = 0; idx < cell_count; idx++) {
        screen->cells[idx].glyph = 0;
        screen->cells[idx].bg_color = screen->bg_color;
    }
}

console_cell_t *console_screen_cell(const console_screen_t *screen, const uint32_t x, const uint32_t y) {
    return &screen->cells[(y * screen->width) + x];
}

void console_screen_put_view_at(console_screen_t *screen, console_view_t *view, uint32_t x, uint32_t y) {
    console_rect_t rect = {x, y, view->width, view->height};
    console_screen_set_cells(screen, &rect, view->cells);
}

void console_screen_set_cell(console_screen_t *screen, uint32_t x, uint32_t y, console_cell_t cell) {
    screen->cells[(y * screen->width) + x] = cell;
}

void console_screen_set_cells(console_screen_t *screen, console_rect_t *rect, console_cell_t *cells) {
    uint32_t cell_count = rect->width * rect->height;
    for (uint32_t cell_idx = 0; cell_idx < cell_count; cell_idx++) {
        uint32_t x = rect->x + (cell_idx % rect->width);
        uint32_t y = rect->y + (cell_idx / rect->width);
        screen->cells[(y * screen->width) + x] = cells[cell_idx];
    }
}


/* Console Views */

console_view_t *console_view_from_rexfile(const char *filename) {
    
    rex_tile_map_t *map = rex_load_tile_map(filename);
    if (!map) {
        return NULL;
    }

    rex_tile_layer_t *layer = rex_flatten_tile_map(map);
    if (!layer) {
        return NULL;
    }

    console_view_t *v = calloc(1, sizeof(console_view_t));
    v->width = layer->width;
    v->height = layer->height;
    uint32_t tile_count = v->width * v->height;
    v->cells = calloc(tile_count, sizeof(console_cell_t));

    for (uint32_t t = 0; t < tile_count; t++) {
        uint32_t cell_idx = view_cell_index_for_rex_index(t, layer->width, layer->height);
        console_cell_t *cell = &v->cells[cell_idx];
        rex_tile_t tile = layer->tiles[t];
        cell->glyph = tile.char_code;
        if (!rex_tile_is_transparent(&tile)) {
            cell->fg_color = COLOR_FROM_RGBA(tile.fg_red, tile.fg_green, tile.fg_blue, 255);
            cell->bg_color = COLOR_FROM_RGBA(tile.bg_red, tile.bg_green, tile.bg_blue, 255);
        } else {
            // Transparent tile
            cell->fg_color = COLOR_FROM_RGBA(tile.fg_red, tile.fg_green, tile.fg_blue, 255);
            cell->bg_color = COLOR_FROM_RGBA(0, 0, 0, 0);
        }
    }
    
    rex_destroy_tile_map(map);

    return v;
}

void console_view_destroy(console_view_t *view) {
    free(view->cells);
    free(view);
}


// Internal Functions --

/*
 * Return the corresponding index of the cell in the console image cell array for the given index in the
 * rex_tile_layer tile array. The console image will be stored in row-major order, while the rex_layer data
 * is stored in column-major order. 
 */
static 
uint32_t view_cell_index_for_rex_index(const uint32_t rex_idx, const uint32_t width, const uint32_t height) {
    uint32_t row = rex_idx % height;
    uint32_t col = rex_idx / height;
    return (row * width) + col;
}


