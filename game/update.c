#include "game.h"


#define _CAMERA_MOVE_SPEED_ (0.5)


void update() {
    switch (page) {
        case PAGE_IN_GAME:
            update_game();
            break;
        
        case PAGE_MAIN_MENU:
            break;
    }

    // update alerts' time to live
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live > 0) {
            alerts[i].time_to_live -= delta_time;
        }
    }
    
    return;
}

void update_game() {
    #ifdef DEBUG_SOFT_MODE
    if (keys[SDL_SCANCODE_K] == 1) {
        audio_sound_play(wheight_down_sound);
        game_struct.players[0].wheight -= 1;
        game_struct.players[1].wheight += 1;
    }
    if (keys[SDL_SCANCODE_L] == 1) {
        audio_sound_play(wheight_up_sound);
        game_struct.players[0].wheight += 1;
        game_struct.players[1].wheight -= 1;
    }
    #endif
    if (keys[SDL_SCANCODE_J] == 1) {
        request_switch_turn();
    }

    if (
        game_struct.game_ended == 0 &&
        (
            game_struct.players[0].wheight >= _WIN_WHEIGHT_ ||
            game_struct.players[1].wheight >= _WIN_WHEIGHT_
        )
    ) {
        game_struct.game_ended = 1;
        
        // game ended - ui list
        char* message;
        if (game_struct.players[0].wheight >= _WIN_WHEIGHT_) {
            message = "You won!";
            audio_sound_play(win_game_sound);
        }else {
            message = "You lost";
            audio_sound_play(lose_game_sound);
        }
        int32_t ui_list_id = new_ui_list_assign_id();
        ui_lists[ui_list_id] = (ui_list_t){
            .active = 1,
            .permenant = 1,

            .font = &big_letters_font,
            .padding = 1,
            .button_padding = 4,
            .box_nslice = &nine_slice2,
            .button_hover_nslice = &nine_slice3,
            .info_string_nslice = &nine_slice1,

            .box_pos_from_world_pos = 0,
            .x = _OUTPORT_WIDTH_*0.5  - (max(strlen(message), strlen("exit game"))*big_letters_font.letter_width + 4*2)*0.5,
            .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height + 4*2,

            .buttons_amount = 2,
            .button_strings = {
                "exit game",
                message
            },
            .button_info_strings = {"",""},
            .button_callbacks = {
                &ui_list_exit_game_button_callback,
                NULL
            },

            .child_ui_list = -1,
            .parent_ui_list = -1
        };
    }

    player_translations_update();
    tile_cooldowns_update();
    camera_update();
    sun_shadow_map_update(); // a bit redundent
    update_hovered_tile();
    
    if (game_struct.player_turn == 1) {
        player_1_turn();
    }
}

void camera_update() {
    #ifdef DEBUG_SOFT_MODE
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
    #endif


    #ifndef DEBUG_SOFT_MODE
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
    #endif


    // follow camera_pos
    camera->x = camera_pos.x - 60*cos(camera->ry+M_PI*0.5)*cos(camera->rx);
    camera->y = camera_pos.y - 60*sin(camera->rx);
    camera->z = camera_pos.z - 60*sin(camera->ry+M_PI*0.5)*cos(camera->rx);

    #ifdef DEBUG_SOFT_MODE
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
    #endif
}

void player_translations_update() {
    in_tiles_translation = 0;
    
    for (uint8_t i = 0; i < 2; i++) {
        // set translation
        int32_t showed_wheight = game_struct.players[i].wheight;
        if (showed_wheight > _WIN_WHEIGHT_) {
            showed_wheight = _WIN_WHEIGHT_;
        }else if (showed_wheight < -_WIN_WHEIGHT_) {
            showed_wheight = -_WIN_WHEIGHT_;
        }

        float last_y_translation = game_struct.players[i].y_translation;

        game_struct.players[i].y_translation = (-showed_wheight*0.5)*_TILE_SIZE_+_SCALE_AXIS_POINT_Y_ + _TILE_SIZE_*0.5;
        if (last_y_translation != game_struct.players[i].y_translation) {
            in_tiles_translation = 1;

            game_struct.players[i].y_lerp_start_translation = game_struct.players[i].y_current_translation;
            game_struct.players[i].translation_lerp_time = 0;
        }

        // animate current translation
        if (game_struct.players[i].y_current_translation != game_struct.players[i].y_translation) {
            in_tiles_translation = 1;

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

void tile_cooldowns_update() {
    in_cooldowns_translation = 0;

    for (uint8_t i = 0; i < 2; i++) {
        for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
            for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                tile_t* tile = &(game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
                
                if (tile->type == TILE_TYPE_EMPTY) continue;
                if (tile->curent_cooldown_timer == tile->cooldown_timer) continue;

                in_cooldowns_translation = 1;

                if (tile->cooldown_timer > tile->curent_cooldown_timer) {
                    // go to 0 and jump up
                    tile->curent_cooldown_timer -= ((float)delta_time) / _PLAYER_COOLDOWN_TRANSLATION_LERP_DURATION_;
                    if (tile->curent_cooldown_timer <= 0) {
                        tile->curent_cooldown_timer = tile->cooldown_timer;
                        // add resources
                        if (i == 0) {
                            audio_sound_play(resource_give_sound);
                        }
                        
                        tile_type_t* tile_type = &(tile_type_properties[tile->type]);
                        game_struct.players[i].resources.population += tile_type->give.population;
                        game_struct.players[i].resources.wheat      += tile_type->give.wheat;
                        game_struct.players[i].resources.wood       += tile_type->give.wood;
                        game_struct.players[i].resources.stone      += tile_type->give.stone;
                        game_struct.players[i].resources.soldiers   += tile_type->give.soldiers;
                        // add alert
                        int32_t new_alert_id = new_alert_assign_id();
                        alerts[new_alert_id] = (alert_t){
                            .time_to_live = 3000,

                            .font = &letters_font,
                            .padding = 3,

                            .initial_time_to_live = 3000,
                            .y_full_transform = letters_font.letter_height*3,
                            .easing_function = &ease_out_sine,

                            .box_pos_from_world_pos = 1,
                            .box_world_pos_x = 0.5*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            .box_world_pos_y =   1*_TILE_SIZE_ + game_struct.players[i].y_current_translation,
                            .box_world_pos_z = 0.5*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
                            .x = 0,
                            .y = 0,
                            
                            .string = tile_type->give_alert_string
                        };
                    }
                }else {
                    // go down
                    tile->curent_cooldown_timer -= ((float)delta_time) / _PLAYER_COOLDOWN_TRANSLATION_LERP_DURATION_;
                    if (tile->curent_cooldown_timer <= tile->cooldown_timer) {
                        tile->curent_cooldown_timer = tile->cooldown_timer;
                    }
                }
            }
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