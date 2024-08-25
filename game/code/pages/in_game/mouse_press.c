#include "page.h"
#include "../../sounds.h"
#include "../../ui_lists.h"
#include "../../alerts.h"


void mouse_press_in_game() {
    if (game_struct.game_ended == 1) {
        add_error_alert_at_cursor("The game has ended");
        return;
    }
    
    if (game_struct.player_turn == 1) {
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
            int32_t ui_list_id = new_ui_list_billboarded(
                selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation + _TILE_SIZE_*0.5,
                game_struct.players[0].y_current_translation,
                selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                camera,
                0,
                0,
                1,
                &nine_slice2
            );
            ui_list_add_element(
                ui_list_id,
                (ui_list_element_t){
                    .type = ELEMENT_TYPE_BUTTON,
                    .button = {
                        .callback = &build_button_callback,
                        .string = "Build",
                        .font = &letters_font,
                        .padding = 2,
                        .nslice = NULL,
                        .hover_nslice = &nine_slice3,
                        .info_string = "",
                        .info_string_font = NULL,
                        .info_string_padding = 0,
                        .info_string_nslice = NULL
                    }
                }
            );
        }else {
            // destroy

            // create ui list
            int32_t ui_list_id = new_ui_list_billboarded(
                selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation + _TILE_SIZE_*0.5,
                game_struct.players[0].y_current_translation,
                selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                camera,
                0,
                0,
                1,
                &nine_slice2
            );
            ui_list_add_element(
                ui_list_id,
                (ui_list_element_t){
                    .type = ELEMENT_TYPE_BUTTON,
                    .button = {
                        .callback = &demolish_button_callback,
                        .string = "Demolish",
                        .font = &letters_font,
                        .padding = 2,
                        .nslice = NULL,
                        .hover_nslice = &nine_slice3,
                        .info_string = tile_type_properties[tile_type_id].demolish_info_string,
                        .info_string_font = &letters_font,
                        .info_string_padding = 3,
                        .info_string_nslice = &nine_slice1
                    }
                }
            );
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
            int32_t ui_list_id = new_ui_list_billboarded(
                selected_tile.x*_TILE_SIZE_ + game_struct.players[1].x_current_translation + _TILE_SIZE_*0.5,
                game_struct.players[1].y_current_translation,
                selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
                camera,
                0,
                0,
                1,
                &nine_slice2
            );
            ui_list_add_element(
                ui_list_id,
                (ui_list_element_t){
                    .type = ELEMENT_TYPE_BUTTON,
                    .button = {
                        .callback = &attack_button_callback,
                        .string = "Attack",
                        .font = &letters_font,
                        .padding = 2,
                        .nslice = NULL,
                        .hover_nslice = &nine_slice3,
                        .info_string = "Destroy this building\n* -1\x19\x1a",
                        .info_string_font = &letters_font,
                        .info_string_padding = 3,
                        .info_string_nslice = &nine_slice1
                    }
                }
            );
        }
    }
}