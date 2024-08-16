#include "game.h"


#define _CAMERA_MOVE_SPEED_ (0.5)


void update() {
    update_game();
    
    return;
}

void update_game() {
    camera_update();
    sun_shadow_map_update();
}

void camera_update() {
    // move left right
    if (keys[SDL_SCANCODE_A]) {
        camera_pos.x += _CAMERA_MOVE_SPEED_*cos(camera->ry+M_PI);
        camera_pos.z += _CAMERA_MOVE_SPEED_*sin(camera->ry+M_PI);
    }
    if (keys[SDL_SCANCODE_D]) {
        camera_pos.x += _CAMERA_MOVE_SPEED_*cos(camera->ry);
        camera_pos.z += _CAMERA_MOVE_SPEED_*sin(camera->ry);
    }
    
    // move in out
    if (keys[SDL_SCANCODE_S]) {
        camera_pos.x += _CAMERA_MOVE_SPEED_*cos(camera->ry+M_PI*1.5);
        camera_pos.z += _CAMERA_MOVE_SPEED_*sin(camera->ry+M_PI*1.5);
    }
    if (keys[SDL_SCANCODE_W]) {
        camera_pos.x += _CAMERA_MOVE_SPEED_*cos(camera->ry+M_PI*0.5);
        camera_pos.z += _CAMERA_MOVE_SPEED_*sin(camera->ry+M_PI*0.5);
    }
    
    // move up down
    if (keys[SDL_SCANCODE_SPACE]) {
        camera_pos.y += _CAMERA_MOVE_SPEED_;
    }
    if (keys[SDL_SCANCODE_LCTRL]) {
        camera_pos.y -= _CAMERA_MOVE_SPEED_;
    }


    // X axis rotation
    if (keys[SDL_SCANCODE_KP_2]) {
        camera->rx -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_8]) {
        camera->rx += 0.05*delta_frames;
    }

    // Z axis rotation
    if (keys[SDL_SCANCODE_KP_7]) {
        camera->rz -= 0.01*delta_frames;
    }else if (keys[SDL_SCANCODE_KP_9]) {
        camera->rz += 0.01*delta_frames;
    }else {
        camera->rz = 0;
    }

    // Y axis rotation
    if (keys[SDL_SCANCODE_KP_6]) {
        camera->ry -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_4]) {
        camera->ry += 0.05*delta_frames;
    }


    // follow camera_pos
    camera->x = camera_pos.x      - 60*cos(camera->ry+M_PI*0.5)*cos(camera->rx);
    camera->y = camera_pos.y + 20 - 60*sin(camera->rx);
    camera->z = camera_pos.z      - 60*sin(camera->ry+M_PI*0.5)*cos(camera->rx);

    // perspective <-> orthographic togle
    if (keys[SDL_SCANCODE_P] == 1) {
        if (camera->is_prespective) {
            camera->is_prespective = 0;
            camera->near = -32000;
        } else {
            camera->is_prespective = 1;
            camera->near = 0.01;
        }
    }
}

void sun_shadow_map_update() {
    // follow the player
    sun_shadow_map_camera->x = 0;
    sun_shadow_map_camera->y = 0;
    sun_shadow_map_camera->z = 0;

    sun_shadow_map_camera->ry += 0.002*delta_frames;
    sun_vector_x = cos(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
    sun_vector_y = sin(sun_shadow_map_camera->rx);
    sun_vector_z = sin(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
}