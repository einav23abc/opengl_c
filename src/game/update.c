#include "game.h"

void game_update() {
    player_update();
    player_camera_update();
    sun_shadow_map_update();

    return;
}

void player_update() {
    player.vx = 0;
    player.vz = 0;

    // move left right (set velocity)
    if (keys[SDL_SCANCODE_A]) {
        player.vx += 2*cos(player_camera->ry+M_PI);
        player.vz += 2*sin(player_camera->ry+M_PI);
    }
    if (keys[SDL_SCANCODE_D]) {
        player.vx += 2*cos(player_camera->ry);
        player.vz += 2*sin(player_camera->ry);
    }
    
    // move in out (set velocity)
    if (keys[SDL_SCANCODE_S]) {
        player.vx += 2*cos(player_camera->ry+M_PI*1.5);
        player.vz += 2*sin(player_camera->ry+M_PI*1.5);
    }
    if (keys[SDL_SCANCODE_W]) {
        player.vx += 2*cos(player_camera->ry+M_PI*0.5);
        player.vz += 2*sin(player_camera->ry+M_PI*0.5);
    }

    // gravity
    player.vy -= 0.25*delta_frames;
    // jump
    if (keys[SDL_SCANCODE_SPACE] && player.can_jump_buffer > 0) {
        player.vy = 5;
        player.can_jump_buffer = 0;
    }

    // move
    player.cube.x += player.vx*delta_frames;
    player.cube.y += player.vy*delta_frames;
    player.cube.z += player.vz*delta_frames;
    
    // update ry appropriate to move direction
    if (player.vz != 0 || player.vx != 0) {
        while (player.cube.ry >= M_PI*2) {player.cube.ry -= M_PI*2;}
        while (player.cube.ry < 0) {player.cube.ry += M_PI*2;}

        float  goal_ry = atan(player.vz/player.vx)+M_PI*1.5;
        if (player.vx < 0) {
            goal_ry += M_PI;
        }

        while (goal_ry >= M_PI*2) {goal_ry -= M_PI*2;}
        while (goal_ry < 0) {goal_ry += M_PI*2;}
        
        // +-2PI for smallest diffrence
        if (abs(player.cube.ry-(goal_ry-M_PI*2)) < abs(player.cube.ry-goal_ry)) {
            goal_ry -= M_PI*2;
        }
        if (abs(player.cube.ry-(goal_ry+M_PI*2)) < abs(player.cube.ry-goal_ry)) {
            goal_ry += M_PI*2;
        }

        float delta_ry = 0.15 * delta_frames;
        player.cube.ry = player.cube.ry*(1.0-delta_ry) + goal_ry*(delta_ry);
    }

    cube_update_aabb(&(player.cube));

    // update can_jump_buffer
    if (player.can_jump_buffer > 0) {
        player.can_jump_buffer -= 1*delta_frames;
    }

    // <collision>
    vec3_t resolution;
    for (uint32_t i = 0; i < CUBES_AMOUNT; i++) {
        resolution = sat_cube_collision(&(player.cube), &(cubes[i]));
        if (resolution.x != 0 || resolution.y != 0 || resolution.z != 0) {
            player.cube.x += resolution.x;
            player.cube.y += resolution.y;
            player.cube.z += resolution.z;
        }

        // set can_jump_buffer
        if (resolution.y > 0) {
            player.can_jump_buffer = 10;
            // hit the floor - reset vy
            player.vy = 0;
        }
    }
    // </collision>
}

void player_camera_update() {
    // X axis rotation
    if (keys[SDL_SCANCODE_KP_2]) {
        player_camera->rx -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_8]) {
        player_camera->rx += 0.05*delta_frames;
    }

    // Z axis rotation
    if (keys[SDL_SCANCODE_KP_7]) {
        player_camera->rz -= 0.01*delta_frames;
    }else if (keys[SDL_SCANCODE_KP_9]) {
        player_camera->rz += 0.01*delta_frames;
    }else {
        player_camera->rz = 0;
    }

    // Y axis rotation
    if (keys[SDL_SCANCODE_KP_6]) {
        player_camera->ry -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_4]) {
        player_camera->ry += 0.05*delta_frames;
    }

    // follow the player
    player_camera->x = player.cube.x      - 60*cos(player_camera->ry+M_PI*0.5)*cos(player_camera->rx);
    player_camera->y = player.cube.y + 20 - 60*sin(player_camera->rx);
    player_camera->z = player.cube.z      - 60*sin(player_camera->ry+M_PI*0.5)*cos(player_camera->rx);

    // perspective <-> orthographic togle
    if (keys[SDL_SCANCODE_P] == 1) {
        if (player_camera->is_prespective) {
            player_camera->is_prespective = 0;
            player_camera->near = -32000;
        } else {
            player_camera->is_prespective = 1;
            player_camera->near = 0.01;
        }
    }
}

void sun_shadow_map_update() {
    // follow the player
    sun_shadow_map_camera->x = player.cube.x;
    sun_shadow_map_camera->y = player.cube.y;
    sun_shadow_map_camera->z = player.cube.z;

    // int32_t time = SDL_GetTicks()

    sun_shadow_map_camera->ry += 0.002*delta_frames;
    sun_vector_x = cos(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
    sun_vector_y = sin(sun_shadow_map_camera->rx);
    sun_vector_z = sin(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
}