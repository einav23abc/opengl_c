#ifndef GAME_TEXTURES_H
#define GAME_TEXTURES_H

#include "../../engine/engine.h"

extern texture_t* floor_texture;
extern texture_t* global_texture;
extern texture_t* tile_texture;
extern texture_t* field_tile_texture;
extern texture_t* forest_tile_texture;
extern texture_t* mine_tile_texture;
extern texture_t* house_tile_texture;
extern texture_t* barracks_tile_texture;
extern texture_t* attack_effect_texture;
extern texture_t* letters_font_texture;
extern texture_t* big_letters_font_texture;
extern texture_t* nine_slice1_texture;
extern texture_t* nine_slice2_texture;
extern texture_t* nine_slice3_texture;
extern texture_t* menu_background_scroll_texture;

void game_textures_init();

#endif