#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "game.h"

typedef struct {
    uint8_t box_pos_from_world_pos : 1;
    float box_world_pos_x;
    float box_world_pos_y;
    float box_world_pos_z;
    // if box_pos_from_world_pos = 1, this is a screen-x translation
    int32_t x;
    // if box_pos_from_world_pos = 1, this is a screen-y translation
    int32_t y;

    int32_t box_width;
    int32_t box_height;
} billboard_t;

uvec2_t get_billboard_box_pos(billboard_t billboard);
/* \brief Draw a ui billboard element with a shader.
 * 
 * Assumes the shader's uniform0 is vec2 u_position.
 * Assumes the shader's uniform1 is vec2 u_scale.
 */
void draw_billboard_shaded(billboard_t billboard);


#endif