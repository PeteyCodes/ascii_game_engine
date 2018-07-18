#ifndef CONSOLE_H
#define CONSOLE_H


#include <stdint.h>
#include <SDL2/SDL.h>

#include "list.h"


// Helper macros for working with pixel colors
#define RED(c) ((c & 0xff000000) >> 24)
#define GREEN(c) ((c & 0x00ff0000) >> 16)
#define BLUE(c) ((c & 0x0000ff00) >> 8)
#define ALPHA(c) (c & 0xff)

#define COLOR_FROM_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)


typedef struct {
    uint32_t x;
    uint32_t y;
} console_point_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} console_rect_t;

typedef struct {
    uint32_t glyph;
    uint32_t fg_color;
    uint32_t bg_color;
} console_cell_t;

typedef struct {
    /* All values measured in cells */
    uint32_t width;
    uint32_t height;
    console_cell_t *cells;
} console_view_t;

typedef struct {
    /* All values measured in cells */
    uint32_t width;     
    uint32_t height;    
    uint32_t bg_color;
    console_cell_t *cells;
} console_screen_t;

typedef struct {
    uint32_t width;         // pixels
    uint32_t height;        // pixels
    uint32_t row_count;     // cells
    uint32_t col_count;     // cells
    uint32_t cell_width;    // pixels
    uint32_t cell_height;   // pixels
    uint32_t bg_color;
    SDL_Renderer *renderer;
    SDL_Texture *font_texture;
} console_t;


/* Console Management */

console_t * console_create(SDL_Window *window, 
        uint32_t width, uint32_t height, 
        uint32_t row_count, uint32_t col_count,
        uint32_t bg_color, const char *font_filename);

void console_destroy(console_t *console);

void console_clear(console_t *console);

void console_render_screen(console_t *console, console_screen_t *screen);


/* Console Screens */
console_screen_t * console_screen_create(uint32_t width, uint32_t height, uint32_t bg_color);

void console_screen_destroy(console_screen_t *screen);

void console_screen_clear(console_screen_t *screen);

console_cell_t *console_screen_cell(const console_screen_t *screen, const uint32_t x, const uint32_t y);

void console_screen_put_text_at(console_screen_t *screen, const char *text, console_rect_t recti, uint32_t fg_color, uint32_t bg_color);

void console_screen_put_view_at(console_screen_t *screen, console_view_t *view, uint32_t x, uint32_t y);

void console_screen_set_cell(console_screen_t *screen, uint32_t x, uint32_t y, console_cell_t cell);

void console_screen_set_cells(console_screen_t *screen, console_rect_t *rect, console_cell_t *cells);


/* Console Views */

console_view_t *console_view_from_rexfile(const char *filename);

void console_view_destroy(console_view_t *view);



#endif


