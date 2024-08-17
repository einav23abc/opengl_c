// this is included inside "handle_event.c"
#include "game.h"

void ui_list_build_house_button_callback(int32_t ui_list_id) {
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x] = 1;
    selected_tile.x = -1;
    selected_tile.y = -1;
    close_all_ui_lists();
}

void ui_list_build_button_callback(int32_t ui_list_id) {
    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = ui_lists[ui_list_id].box_world_pos_x,
        .box_world_pos_y = ui_lists[ui_list_id].box_world_pos_y,
        .box_world_pos_z = ui_lists[ui_list_id].box_world_pos_z,
        .x = get_ui_list_width(ui_list_id) + 2*_UI_LIST_PADDING_,
        .y = 0,

        .buttons_amount = 1,
        .button_strings = {"house"},
        .button_info_strings = {"this is a info string\nnew line"},
        .button_callbacks = {&ui_list_build_house_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}