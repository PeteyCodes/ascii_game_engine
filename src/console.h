#ifndef CONSOLE_H
#define CONSOLE_H


#include <stdint.h>


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
    console_cell_t *cells;
} console_image_t;


console_image_t *image_from_rexfile(const char *filename);
void image_destroy(console_image_t *image);


#endif


