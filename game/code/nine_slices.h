#ifndef NINE_SLICES_H
#define NINE_SLICES_H

#include "../../engine/engine.h"
#include "meshes.h"
#include "shaders.h"

typedef struct {
    int32_t texture_width;
    int32_t texture_height;
    int32_t padding;
    texture_t* texture;
} nine_slice_t;

void draw_nine_slice(nine_slice_t nine_slice, int32_t x, int32_t y, int32_t width, int32_t height);

#endif