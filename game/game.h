#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"


#define _PLAYER_GRID_WIDTH_ (4)
#define _PLAYER_GRID_DEPTH_ (4)
#define _TILE_WIDTH_ (32)
#define _TILE_HEIGHT_ (32)
#define _TILE_DEPTH_ (32)

typedef struct {

} tile_type_t;

typedef struct {
    int32_t tiles[_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_];
} player_t;

typedef struct {
    player_t players[2];
} game_t;


#define _OUTPORT_WIDTH_ (320*3)
#define _OUTPORT_HEIGHT_ (240*3)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


extern fbo_t* outport_fbo;

extern texture_t* global_texture;
extern shader_t* global_shader;

extern vec3_t camera_pos;
extern camera_t* camera;

extern mesh_t* cube_mesh;

extern game_t game_struct;

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;
extern shader_t* sun_shadow_map_shader;




void update_game();
void render_game();

void camera_update();

void sun_shadow_map_update();

void render_game_world();


#endif