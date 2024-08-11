#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"
#include "config.h"

#ifdef DEBUG_MODE
#define DEBUG_SOFT_MODE
#endif


typedef struct {
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

typedef struct {
    uint8_t connected;

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

#pragma pack(1)
typedef struct {
    float x;
    float y;
    float z;
    float rx;
    float ry;
    float rz;
    float vx;
    float vy;
    float vz;
} update_packet_body_t;
#pragma pack()


extern float frames;

extern uint8_t in_game;
extern uint32_t load_game_progress;
extern thread_t load_game_thread;

extern camera_t* default_camera;

extern fbo_t* outport_fbo;


// <game variables>
    extern texture_t* global_texture;
    extern shader_t* global_shader;

    extern int32_t client_id;
    extern int8_t is_server_host;
    extern float update_packet_delta_frames;

    extern camera_t* player_camera;
    extern int32_t player_id;
    extern player_t players[_PLAYERS_MAX_AMOUNT_]; // the netframe shared object
    extern player_t* player;

    extern mesh_t* man_mesh;
    extern animation_t* man_anim_t_pose;
    extern animation_t* man_anim_run;

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



uint8_t init();
void load_game();
void init_shared_object_players();
void init_player_data(int32_t player_id);

void handle_event();

void update();
void update_game();
void load_game_update();
vec3_t sat_cube_collision(cube_t* cube1, cube_t* cube2);
void cube_update_aabb(cube_t* cube);
void players_update();
void player_camera_update();
void sun_shadow_map_update();

void render();
void render_game();
void render_load_game_screen();
void render_game_world();
void cube_draw(cube_t* cube);
void cube_debug_draw_vertices(cube_t* cube);

// netframe functions in game.c
server_packet_t generate_state_packet();
void parse_state_packet(server_packet_t packet);
void parse_update_packet(server_packet_t packet);
void handle_client_connect(int32_t client_id);
void handle_client_disconnect(int32_t client_id);
void handle_disconnect_as_client();



#endif