#ifndef GAME_TYPES_H
#define GAME_TYPES_H

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
} player_t;

#endif