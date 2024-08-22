#include "page.h"


void mouse_press_in_game() {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        return;
    }
    
    if (game_struct.player_turn == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("It is not your turn");
        return;
    }

    // find selected tile
    ivec2_t pressed_on_tile = get_hovered_tile_position(0);
    if (
        pressed_on_tile.x < 0 ||
        pressed_on_tile.x >= _PLAYER_GRID_WIDTH_ ||
        pressed_on_tile.y < 0 ||
        pressed_on_tile.y >= _PLAYER_GRID_DEPTH_
    ) {
        pressed_on_tile = get_hovered_tile_position(1);
        if (
            pressed_on_tile.x < 0 ||
            pressed_on_tile.x >= _PLAYER_GRID_WIDTH_ ||
            pressed_on_tile.y < 0 ||
            pressed_on_tile.y >= _PLAYER_GRID_DEPTH_
        ) {
            selected_tile.x = -1;
            selected_tile.y = -1;
            selected_tile.z = -1;
        }else {
            selected_tile.x = pressed_on_tile.x;
            selected_tile.y = pressed_on_tile.y;
            selected_tile.z = 1;
        }
    }else {
        selected_tile.x = pressed_on_tile.x;
        selected_tile.y = pressed_on_tile.y;
        selected_tile.z = 0;
    }


    // in player 0
    if (selected_tile.z == 0) {
        audio_sound_play(select_tile_sound);

        int32_t tile_type_id = game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type;

        if (tile_type_id == TILE_TYPE_EMPTY) {
            // build

            // create ui list
            int32_t ui_list_id = new_ui_list_assign_id();
            ui_lists[ui_list_id] = (ui_list_t){
                .active = 1,
                .permenant = 0,

                .hidden = 0,

                .font = &letters_font,
                .info_string_font = &letters_font,
                .padding = 1,
                .button_padding = 2,
                .box_nslice = &nine_slice2,
                .button_hover_nslice = &nine_slice3,
                .info_string_nslice = &nine_slice1,

                .box_pos_from_world_pos = 1,
                .box_world_pos_x = selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation + _TILE_SIZE_*0.5,
                .box_world_pos_y = game_struct.players[0].y_current_translation,
                .box_world_pos_z = selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                .x = 0,
                .y = 0,

                .buttons_amount = 1,
                .button_strings = {"Build"},
                .button_info_strings = {""},
                .button_callbacks = {&build_button_callback},

                .child_ui_list = -1,
                .parent_ui_list = -1
            };
        }else {
            // destroy

            // create ui list
            int32_t ui_list_id = new_ui_list_assign_id();
            ui_lists[ui_list_id] = (ui_list_t){
                .active = 1,
                .permenant = 0,

                .hidden = 0,

                .font = &letters_font,
                .info_string_font = &letters_font,
                .padding = 1,
                .button_padding = 2,
                .box_nslice = &nine_slice2,
                .button_hover_nslice = &nine_slice3,
                .info_string_nslice = &nine_slice1,

                .box_pos_from_world_pos = 1,
                .box_world_pos_x = selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation + _TILE_SIZE_*0.5,
                .box_world_pos_y = game_struct.players[0].y_current_translation,
                .box_world_pos_z = selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                .x = 0,
                .y = 0,

                .buttons_amount = 1,
                .button_strings = {"Demolish"},
                .button_info_strings = {tile_type_properties[tile_type_id].demolish_info_string},
                .button_callbacks = {&demolish_button_callback},

                .child_ui_list = -1,
                .parent_ui_list = -1
            };
        }
    }else if (selected_tile.z == 1) {
        int32_t tile_type_id = game_struct.players[1].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type;

        if (tile_type_id == TILE_TYPE_EMPTY) {
            selected_tile.x = -1;
            selected_tile.y = -1;
            selected_tile.z = -1;
        }else {
            audio_sound_play(select_tile_sound);

            // attack other player

            // create ui list
            int32_t ui_list_id = new_ui_list_assign_id();
            ui_lists[ui_list_id] = (ui_list_t){
                .active = 1,
                .permenant = 0,

                .hidden = 0,

                .font = &letters_font,
                .info_string_font = &letters_font,
                .padding = 1,
                .button_padding = 2,
                .box_nslice = &nine_slice2,
                .button_hover_nslice = &nine_slice3,
                .info_string_nslice = &nine_slice1,

                .box_pos_from_world_pos = 1,
                .box_world_pos_x = selected_tile.x*_TILE_SIZE_ + game_struct.players[1].x_current_translation + _TILE_SIZE_*0.5,
                .box_world_pos_y = game_struct.players[1].y_current_translation,
                .box_world_pos_z = selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                .x = 0,
                .y = 0,

                .buttons_amount = 1,
                .button_strings = {"Attack"},
                .button_info_strings = {"Destroy this building\n* -1\x19\x1a"},
                .button_callbacks = {&attack_button_callback},

                .child_ui_list = -1,
                .parent_ui_list = -1
            };
        }
    }
}