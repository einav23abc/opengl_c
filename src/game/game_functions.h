#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "../engine/engine.h"

#include "game_types.h"

void game_render_world();

void cube_update_aabb(cube_t* cube);
void cube_draw(cube_t* cube);
void cube_debug_draw_vertices(cube_t* cube);

vec3_t vec3_cross_product(vec3_t vec1, vec3_t vec2);
vec3_t sat_cube_collision(cube_t* cube1, cube_t* cube2);

void player_camera_update();
void player_update();

void sun_shadow_map_update();

#endif