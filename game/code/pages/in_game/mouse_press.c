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
            return;
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
    
    audio_sound_play(select_tile_sound);

    tile_t* tile = &(game_struct.players[selected_tile.z].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x]);

    // no options on enemy empty tile
    if (selected_tile.z == 1 && tile->type == TILE_TYPE_EMPTY) {
        selected_tile.x = -1;
        selected_tile.y = -1;
        selected_tile.z = -1;
        return;
    }

    // create options ui list
    int32_t ui_list_id = new_ui_list_billboarded(
        selected_tile.x*_TILE_SIZE_ + game_struct.players[selected_tile.z].x_current_translation + _TILE_SIZE_*0.5,
        game_struct.players[selected_tile.z].y_current_translation,
        selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_ + _TILE_SIZE_*0.5,
        camera,
        0,
        0,
        1,
        &nine_slice2
    );

    // shield option
    if (selected_tile.z == 0 && tile->shielded == 0) {
        // can be of diffrent sizes to fit whole list width
        char* shield_str;

        if (tile->type == TILE_TYPE_EMPTY) {
            // with build option
            shield_str = "Shield";
        }else {
            // with destroy option
            shield_str = "Shield  ";
        }

        ui_list_add_element(
            ui_list_id,
            (ui_list_element_t){
                .type = ELEMENT_TYPE_BUTTON,
                .button = {
                    .callback = &shield_button_callback,
                    .string = shield_str,
                    .font = &letters_font,
                    .padding = 2,
                    .nslice = NULL,
                    .hover_nslice = &nine_slice3,
                    .info_string = "Shield this tile\nThe shield will absorb 1 attack\n\nCosts:\n* -1\x15\x16\n* -1\x17\x18",
                    .info_string_font = &letters_font,
                    .info_string_padding = 3,
                    .info_string_nslice = &nine_slice1
                }
            }
        );
    }

    // build option
    if (selected_tile.z == 0 && tile->type == TILE_TYPE_EMPTY) {
        ui_list_add_element(
            ui_list_id,
            (ui_list_element_t){
                .type = ELEMENT_TYPE_BUTTON,
                .button = {
                    .callback = &build_button_callback,
                    .string = "Build ",
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
    }

    // destroy option
    if (selected_tile.z == 0 && tile->type != TILE_TYPE_EMPTY) {
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
                    .info_string = tile_type_properties[tile->type].demolish_info_string,
                    .info_string_font = &letters_font,
                    .info_string_padding = 3,
                    .info_string_nslice = &nine_slice1
                }
            }
        );
    }

    // attack other player option
    if (selected_tile.z == 1) {
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

    update_costing_ui_list_buttons();
}