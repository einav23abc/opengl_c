#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"


typedef struct cube_t {
    float x;
    float y;
    float z;

    float rx;
    float ry;
    float rz;

    float w;
    float h;
    float d;

    vec3_t vertices[8];
    float aabb_x;
    float aabb_y;
    float aabb_z;
    float aabb_w;
    float aabb_h;
    float aabb_d;
} cube_t;

typedef struct player_t {
    cube_t cube;

    float can_jump_buffer;
    float vx;
    float vy;
    float vz;

    float current_anim_frame;
    float last_anim_frame;
    float anim_transition_frame;
    animation_t* current_anim;
    animation_t* last_anim;
} player_t;



#define _OUTPORT_WIDTH_ (320*3)
#define _OUTPORT_HEIGHT_ (240*3)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


extern fbo_t* outport_fbo;

extern SDL_Surface* global_texture_surface;
extern texture_t* global_texture;
extern shader_t* global_shader;

extern vec3_t camera_pos;
extern camera_t* camera;

extern mesh_t* cube_mesh;
extern cube_t cubes[];

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;
extern shader_t* sun_shadow_map_shader;

extern const uint64_t CUBES_AMOUNT;




void update_game();
void render_game();

void camera_update();

void sun_shadow_map_update();


void render_game_world();

void cube_draw(cube_t* cube);



#endif