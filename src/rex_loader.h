#ifndef REX_LOADER_H
#define REX_LOADER_H

#include <stdbool.h>
#include <stdint.h>


typedef struct {
    uint8_t char_code;
    uint8_t unused_1;
    uint8_t unused_2;
    uint8_t unused_3;
    uint8_t fg_red;
    uint8_t fg_green;
    uint8_t fg_blue;
    uint8_t bg_red;
    uint8_t bg_green;
    uint8_t bg_blue;
} rex_tile_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    rex_tile_t *tiles;      // column-major order
} rex_tile_layer_t;

typedef struct {
    uint32_t version;
    uint32_t layer_count;
    uint32_t width;
    uint32_t height;
    rex_tile_layer_t *layers;
} rex_tile_map_t;

rex_tile_map_t *rex_load_tile_map(char *filename);
void rex_destroy_tile_map(rex_tile_map_t *map);

rex_tile_layer_t *rex_flatten_tile_map(rex_tile_map_t *map);
bool rex_tile_is_transparent(rex_tile_t *tile);

#endif




