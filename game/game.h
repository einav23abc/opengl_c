#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"



#define _OUTPORT_WIDTH_ (320)
#define _OUTPORT_HEIGHT_ (260)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


#define _PLAYER_GRID_WIDTH_ (4)
#define _PLAYER_GRID_DEPTH_ (4)
#define _TILE_SIZE_ (16)
#define _PLAYER_CONSTANT_Z_TRANSLATION_ (-_PLAYER_GRID_DEPTH_*_TILE_SIZE_*0.5)
#define _PLAYER_TRANSLATION_LERP_DURATION_ (500)
#define _WIN_WHEIGHT_ (6)
#define _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_ (2)
#define _SCALE_AXIS_POINT_Y_ (_TILE_SIZE_*(_WIN_WHEIGHT_*0.5))
#define _SCALE_AXIS_LENGTH_ (sqrt(_SCALE_AXIS_POINT_Y_*_SCALE_AXIS_POINT_Y_ + _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_TILE_SIZE_*_TILE_SIZE_))


typedef struct {

} tile_type_t;

typedef struct {
    float y_lerp_start_translation;
    float y_translation;
    float y_current_translation;
    float x_current_translation;
    float hinge_y_position;
    float hinge_x_position;
    uint32_t translation_lerp_time;
    int32_t wheight;
    int32_t tiles[_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_];
} player_t;

typedef struct {
    player_t players[2];
    uint8_t player_turn;
} game_t;



extern fbo_t* outport_fbo;

extern texture_t* floor_texture;
extern texture_t* global_texture;
extern shader_t* global_shader;

extern vec3_t camera_pos;
extern camera_t* camera;

extern mesh_t* cube_mesh;
extern mesh_t* centered_cube_mesh;

extern game_t game_struct;
extern ivec2_t selected_tile;

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;
extern shader_t* sun_shadow_map_shader;




void update_game();
void render_game();

void player_translations_update();
void camera_update();
void sun_shadow_map_update();

void render_game_world();

vec2_t get_mouse_camera_space_position();
vec3_t get_mouse_world_space_position_at_y(float at_y);
ivec2_t get_hovered_tile_position(uint8_t player_i);


#endif