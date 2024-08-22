#include "enter.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

void enter_how_to_play() {
    close_all_ui_lists();
    close_all_alerts();
    page = PAGE_HOW_TO_PLAY;
    int32_t ui_list_id;

    // exit game - ui list
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - (strlen("Main menu")*letters_font.letter_width + 5*2) - 1,
        .y = _OUTPORT_HEIGHT_ - 1,

        .buttons_amount = 1,
        .button_strings = {"Main menu"},
        .button_info_strings = {""},
        .button_callbacks = {&main_menu_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}

void main_menu_button_callback() {
    switch_page(PAGE_MAIN_MENU);
}