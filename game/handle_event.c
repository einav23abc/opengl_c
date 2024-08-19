#include "game.h"

void handle_event() {
    switch(event.type) {
        
        case SDL_MOUSEBUTTONDOWN: {
            // <ui-lists button clicking>
                vec2_t outport_space_position = get_mouse_outport_space_position();
        
                set_ui_lists_to_unsafe();
                ivec3_t in_ui_box = get_ui_list_inside_pos();
                if (in_ui_box.z != -1) {
                    make_ui_list_safe(in_ui_box.z);

                    int32_t in_button = floor(((float)in_ui_box.y)/(ui_lists[in_ui_box.z].font->letter_height + ui_lists[in_ui_box.z].button_padding*2));
                    if (in_button < 0 || in_button >= ui_lists[in_ui_box.z].buttons_amount) return;

                    audio_sound_play(button_press_sound);

                    button_callback_t button_callback = ui_lists[in_ui_box.z].button_callbacks[in_button];
                    if (button_callback != NULL){
                        button_callback(in_ui_box.z, in_button);
                    }

                    close_unsafe_ui_lists();
                    return;
                }
                close_unsafe_ui_lists();
            // </ui-lists button clicking>

            // page specific
            switch(page) {
                case PAGE_IN_GAME:
                    mouse_press_in_game();
                    break;
            }
            break;
        }
    }
    

    return;
}


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

                .font = &letters_font,
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
                .button_strings = {"build"},
                .button_info_strings = {""},
                .button_callbacks = {&ui_list_build_button_callback},

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

                .font = &letters_font,
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
                .button_strings = {"demolish"},
                .button_info_strings = {tile_type_properties[tile_type_id].demolish_info_string},
                .button_callbacks = {&ui_list_demolish_button_callback},

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

                .font = &letters_font,
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
                .button_strings = {"attack"},
                .button_info_strings = {"destroy this building\n* -1\x19\x1a"},
                .button_callbacks = {&ui_list_attack_button_callback},

                .child_ui_list = -1,
                .parent_ui_list = -1
            };
        }
    }


}