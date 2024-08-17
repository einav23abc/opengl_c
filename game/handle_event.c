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

                    int32_t in_button = floor(((float)in_ui_box.y)/_UI_LIST_BUTTON_HEIGHT_);
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

#include "ui_list_button_callbacks.c"

void mouse_press_in_game() {
    if (game_struct.player_turn == 1) {
        close_all_alerts();
        add_alert_at_cursor("It is not your turn.");
        return;
    }

    ivec2_t pressed_on_tile = get_hovered_tile_position(0);
    if (
        pressed_on_tile.x < 0 ||
        pressed_on_tile.x >= _PLAYER_GRID_WIDTH_ ||
        pressed_on_tile.y < 0 ||
        pressed_on_tile.y >= _PLAYER_GRID_DEPTH_
    ) {
        selected_tile.x = -1;
        selected_tile.y = -1;
    }else {
        selected_tile = pressed_on_tile;
        // create ui list
        int32_t ui_list_id = new_ui_list_assign_id();
        ui_lists[ui_list_id] = (ui_list_t){
            .active = 1,

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
    }
}