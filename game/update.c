#include "game.h"


#define _CAMERA_MOVE_SPEED_ (0.5)


void update() {
    update_game();

    // update alerts' time to live
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live > 0) {
            alerts[i].time_to_live -= delta_time;
        }
    }
    
    return;
}

void update_game() {
    if (keys[SDL_SCANCODE_K] == 1) {
        game_struct.players[0].wheight += 1;
        game_struct.players[1].wheight -= 1;
    }
    if (keys[SDL_SCANCODE_L] == 1) {
        game_struct.players[0].wheight -= 1;
        game_struct.players[1].wheight += 1;
    }
    if (keys[SDL_SCANCODE_J] == 1) {
        switch_turn();
    }
    
    if (game_struct.player_turn == 1) {
        player_1_turn();
    }

    player_translations_update();
    camera_update();
    sun_shadow_map_update();
    update_hovered_tile();
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
    camera->x = camera_pos.x - 60*cos(camera->ry+M_PI*0.5)*cos(camera->rx);
    camera->y = camera_pos.y - 60*sin(camera->rx);
    camera->z = camera_pos.z - 60*sin(camera->ry+M_PI*0.5)*cos(camera->rx);

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

void player_translations_update() {
    for (uint8_t i = 0; i < 2; i++) {
        // set translation
        float last_y_translation = game_struct.players[i].y_translation;
        game_struct.players[i].y_translation = (game_struct.players[i].wheight*0.5)*_TILE_SIZE_+_SCALE_AXIS_POINT_Y_ + _TILE_SIZE_*0.5;
        if (last_y_translation != game_struct.players[i].y_translation) {
            game_struct.players[i].y_lerp_start_translation = game_struct.players[i].y_current_translation;
            game_struct.players[i].translation_lerp_time = 0;
        }

        // animate current translation
        if (game_struct.players[i].y_current_translation != game_struct.players[i].y_translation) {
            game_struct.players[i].translation_lerp_time = min(game_struct.players[i].translation_lerp_time + delta_time, _PLAYER_TRANSLATION_LERP_DURATION_);
            float t_linear = ((float)(game_struct.players[i].translation_lerp_time))/_PLAYER_TRANSLATION_LERP_DURATION_;
            float t = ease_out_back(t_linear);
            game_struct.players[i].y_current_translation = (1-t)*(game_struct.players[i].y_lerp_start_translation) + (t)*(game_struct.players[i].y_translation);
        }

        // set hinge_y_position
        game_struct.players[i].hinge_y_position = game_struct.players[i].y_current_translation - _TILE_SIZE_*0.5;
        
        // set hinge_x_position according to hinge_y_position
        game_struct.players[i].hinge_x_position = fabs(
            sqrt(
                (_SCALE_AXIS_LENGTH_*_SCALE_AXIS_LENGTH_) -
                ((_SCALE_AXIS_POINT_Y_-game_struct.players[i].hinge_y_position)*(_SCALE_AXIS_POINT_Y_-game_struct.players[i].hinge_y_position))
            )
        );
        if (i == 0) {
            game_struct.players[i].hinge_x_position = -(game_struct.players[i].hinge_x_position);
        }

        // set x_current_translation
        if (i == 0) {
            game_struct.players[i].x_current_translation = game_struct.players[i].hinge_x_position - _TILE_SIZE_ - _TILE_SIZE_*_PLAYER_GRID_WIDTH_;
        }else {
            game_struct.players[i].x_current_translation = game_struct.players[i].hinge_x_position + _TILE_SIZE_;
        }
    }
}

void update_hovered_tile() {
    ivec3_t in_ui_box = get_ui_list_inside_pos();
    if (in_ui_box.z != -1) {
        hovered_tiles[0].x = -1;
        hovered_tiles[0].y = -1;
        hovered_tiles[1].x = -1;
        hovered_tiles[1].y = -1;
        return;
    }
    
    for (int32_t i = 0; i < 2; i++) {
        hovered_tiles[i] = get_hovered_tile_position(i);
    }
}

void sun_shadow_map_update() {
    // follow the player
    sun_shadow_map_camera->x = 0;
    sun_shadow_map_camera->y = 0;
    sun_shadow_map_camera->z = 0;

    // sun_shadow_map_camera->ry += 0.002*delta_frames;
    sun_vector_x = cos(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
    sun_vector_y = sin(sun_shadow_map_camera->rx);
    sun_vector_z = sin(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
}