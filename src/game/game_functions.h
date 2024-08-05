#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "../engine/engine.h"

#include "game_types.h"

void load_game();

void update_game();
void render_game();


vec3_t sat_cube_collision(cube_t* cube1, cube_t* cube2);


void cube_update_aabb(cube_t* cube);

void player_camera_update();
void player_update();

void sun_shadow_map_update();


void render_load_game_screen();

void render_game_world();

void cube_draw(cube_t* cube);
void cube_debug_draw_vertices(cube_t* cube);


#endif