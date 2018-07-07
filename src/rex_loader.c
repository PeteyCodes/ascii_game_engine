
#include "rex_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include "../lib/zlib.h"



rex_tile_map_t *rex_load_tile_map(char *filename) {

    gzFile file = gzopen(filename, "r");
    if (!file) {
        return NULL;
    }

    // Read tile map information
    rex_tile_map_t *tile_map = calloc(1, sizeof(rex_tile_map_t));
    gzread(file, &tile_map->version, sizeof(tile_map->version));
    gzread(file, &tile_map->layer_count, sizeof(tile_map->layer_count));
    tile_map->layers = calloc(tile_map->layer_count, sizeof(rex_tile_layer_t));

    // Read each layer
    for (int i = 0; i < tile_map->layer_count; i++) {
        rex_tile_layer_t *layer = calloc(1, sizeof(rex_tile_layer_t));
        gzread(file, &layer->width, sizeof(layer->width));
        gzread(file, &layer->height, sizeof(layer->height));

        int32_t tile_count = layer->width * layer->height;
        rex_tile_t *tiles = calloc(tile_count, sizeof(rex_tile_t));
        for (int t = 0; t < tile_count; t++) {
            gzread(file, &tiles[t], sizeof(rex_tile_t));
        }
        layer->tiles = tiles;

        tile_map->layers[i] = *layer;
    }
    
    // Store the width/height information at the map level for convenience
    if (tile_map->layer_count > 0) {
        tile_map->width = tile_map->layers[0].width;
        tile_map->height = tile_map->layers[0].height;
    }
    
    gzclose(file);

    return tile_map;
}

void rex_destroy_tile_map(rex_tile_map_t *map) {
    for (uint32_t l = 0; l < map->layer_count; l++) {
        free(map->layers[l].tiles);
    }
    free(map->layers);
    free(map);
}

rex_tile_layer_t *rex_flatten_tile_map(rex_tile_map_t *map) {

    uint32_t tile_count = map->width * map->height;
    rex_tile_layer_t *working_layer = calloc(tile_count, sizeof(rex_tile_t));
    working_layer->width = map->width;
    working_layer->height = map->height;

    // Flatten all layers down to a single layer
    for (uint32_t l = map->layer_count - 1; l >= 0; l--) {
        rex_tile_layer_t *layer = &map->layers[l];
        for (uint32_t t = 0; t < tile_count; t++) {
            if (!rex_tile_is_transparent(&layer->tiles[t])) {
                working_layer->tiles[t] = layer->tiles[t];
            }
        }        
    }
    
    // TODO: Handle transparent tiles on the flattened tilemap?

    return working_layer;
}

bool rex_tile_is_transparent(rex_tile_t *tile) {
    return (tile->bg_red == 255 && tile->bg_green == 0 && tile->bg_blue == 255);
}






