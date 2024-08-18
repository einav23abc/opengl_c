// this is included inside "handle_event.c"
#include "game.h"

void ui_list_build_specific_button_callback(int32_t ui_list_id, int32_t button_id) {
    int32_t tile_type_id = button_id;
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);

    if (has_enough_resources(0, tile_type_id) == 0) {
        // not enough resources
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }

    game_struct.players[0].wheight += 1;
    game_struct.players[1].wheight -= 1;
    game_struct.players[0].resources.wood       -= tile_type->cost.wood;
    game_struct.players[0].resources.stone      -= tile_type->cost.stone;
    game_struct.players[0].resources.wheat      -= tile_type->cost.wheat;
    game_struct.players[0].resources.population -= tile_type->cost.population;
    game_struct.players[0].resources.soldiers   -= tile_type->cost.soldiers;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].cooldown_timer        = tile_type->give_cooldown;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].curent_cooldown_timer = tile_type->give_cooldown;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type = tile_type_id;
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}

void ui_list_build_button_callback(int32_t ui_list_id, int32_t button_id) {
    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 0,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = ui_lists[ui_list_id].box_world_pos_x,
        .box_world_pos_y = ui_lists[ui_list_id].box_world_pos_y,
        .box_world_pos_z = ui_lists[ui_list_id].box_world_pos_z,
        .x = get_ui_list_width(ui_list_id) + 2*_UI_LIST_PADDING_,
        .y = 0,

        .buttons_amount = _TILE_TYPES_AMOUNT_,
        .button_strings = {"house", "barracks", "field", "mine", "forest"},
        .button_info_strings = {
            tile_type_properties[0].build_info_string,
            tile_type_properties[1].build_info_string,
            tile_type_properties[2].build_info_string,
            tile_type_properties[3].build_info_string,
            tile_type_properties[4].build_info_string
        },
        .button_callbacks = {
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}

void ui_list_attack_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.players[0].resources.soldiers < 1) {
        // not enough resources
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }

    game_struct.players[0].resources.soldiers -= 1;
    game_struct.players[0].wheight += 1;
    game_struct.players[1].wheight -= 1;

    game_struct.players[1].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type = TILE_TYPE_EMPTY;
    game_struct.players[1].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].cooldown_timer        = 0;
    game_struct.players[1].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].curent_cooldown_timer = 0;
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}

void ui_list_demolish_sure_button_callback(int32_t ui_list_id, int32_t button_id) {
    game_struct.players[0].wheight -= 1;
    game_struct.players[1].wheight += 1;

    int32_t tile_type_id = game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type;
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);
    
    // add resources
    game_struct.players[0].resources.wood       += tile_type->give.wood;
    game_struct.players[0].resources.stone      += tile_type->give.stone;
    game_struct.players[0].resources.wheat      += tile_type->give.wheat;
    game_struct.players[0].resources.population += tile_type->give.population;
    game_struct.players[0].resources.soldiers   += tile_type->give.soldiers;
    // add alert
    int32_t new_alert_id = new_alert_assign_id();
    alerts[new_alert_id] = (alert_t){
        .time_to_live = 3000,

        .initial_time_to_live = 3000,
        .y_full_transform = _ALERT_ROW_HEIGHT_*3,
        .easing_function = &ease_out_sine,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = 0.5*_TILE_SIZE_ + selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation,
        .box_world_pos_y =   1*_TILE_SIZE_ + game_struct.players[0].y_current_translation,
        .box_world_pos_z = 0.5*_TILE_SIZE_ + selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
        .x = 0,
        .y = 0,
        
        .string = tile_type->give_alert_string
    };

    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type = TILE_TYPE_EMPTY;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].cooldown_timer        = 0;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].curent_cooldown_timer = 0;
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}

void ui_list_demolish_button_callback(int32_t ui_list_id, int32_t button_id) {
    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 0,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = ui_lists[ui_list_id].box_world_pos_x,
        .box_world_pos_y = ui_lists[ui_list_id].box_world_pos_y,
        .box_world_pos_z = ui_lists[ui_list_id].box_world_pos_z,
        .x = get_ui_list_width(ui_list_id) + 2*_UI_LIST_PADDING_,
        .y = 0,

        .buttons_amount = 2,
        .button_strings = {"yes", "are you sure?"},
        .button_info_strings = {"", ""},
        .button_callbacks = {
            &ui_list_demolish_sure_button_callback,
            NULL
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}