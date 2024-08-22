#ifndef GAME_SHADERS_H
#define GAME_SHADERS_H

#include "../../engine/engine.h"

extern shader_t* font_shader;
extern shader_t* nine_slice_shader;
extern shader_t* global_shader;
extern shader_t* ui_shader;
extern shader_t* cooldown_billboards_shader;
extern shader_t* attacked_billboards_shader;
extern shader_t* tile_effect_shader;
extern shader_t* build_preview_shader;
extern shader_t* sun_shadow_map_shader;

void game_shaders_init();

#endif