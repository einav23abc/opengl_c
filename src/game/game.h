#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"

#include "game_types.h"
#include "game_functions.h"

// externs

// variables declaration
extern float frames;

extern uint8_t in_game;
extern uint32_t load_game_progress;
extern thread_t load_game_thread;

extern camera_t* default_camera;

// <game variables>
    extern texture_t* global_texture;
    extern shader_t* global_shader;

    extern camera_t* player_camera;
    extern player_t player;

    extern mesh_t* man_mesh;
    extern animation_t* anim;

    extern mesh_t* cube_mesh;
    extern cube_t cubes[];

    extern float sun_vector_x;
    extern float sun_vector_y;
    extern float sun_vector_z;
    extern camera_t* sun_shadow_map_camera;
    extern fbo_t* sun_shadow_map_fbo;
    extern shader_t* sun_shadow_map_shader;
// </game variables>

// constants
extern const uint32_t LOAD_GAME_PROGRESS_MAX;
extern const uint64_t CUBES_AMOUNT;

#endif