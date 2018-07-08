
#include "console.h"

#include <stdlib.h>
#include "rex_loader.h"


console_image_t *console_image_from_rexfile(const char *filename) {
    
    rex_tile_map_t *map = rex_load_tile_map(filename);
    if (!map) {
        return NULL;
    }

    rex_tile_layer_t *layer = rex_flatten_tile_map(map);
    if (!layer) {
        return NULL;
    }

    console_image_t *img = calloc(1, sizeof(console_image_t));
    img->width = layer->width;
    img->height = layer->height;
    uint32_t tile_count = img->width * img->height;
    img->cells = calloc(tile_count, sizeof(console_cell_t));

    for (uint32_t t = 0; t < tile_count; t++) {
        console_cell_t *cell = &img->cells[t];
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

    return img;
}

void image_destroy(console_image_t *image) {
    free(image->cells);
    free(image);
}


console_cell_t *console_image_cell(const console_image_t *image, const uint32_t x, const uint32_t y) {
    return &image->cells[(x * image->height) + y];
}


