#ifndef PAGE_IN_GAME_RENDER_H
#define PAGE_IN_GAME_RENDER_H

#include "page.h"

void render_in_game();
void draw_tile(int32_t player_i, int32_t tile_x, int32_t tile_z, int32_t tile_type, int32_t shielded);
void render_game_world();
void render_game_effects();
void render_game_ui();

#endif