#include "page.h"
#include "../in_game/page.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);
static void join_button_callback(int32_t ui_list_id, int32_t button_id);

void enter_join_lan() {
    close_all_ui_lists();
    close_all_alerts();
    
    play_type = PLAY_TYPE_AGAINST_HOST;
    
    int32_t ui_list_id;

    // main menu
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

    // join
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 4,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_*0.5 + (strlen("XXX.XXX.XXX.XXX")*big_letters_font.letter_width + 5*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4,

        .buttons_amount = 1,
        .button_strings = {"Join"},
        .button_info_strings = {""},
        .button_callbacks = {&join_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_MAIN_MENU);
}
static void join_button_callback(int32_t ui_list_id, int32_t button_id) {
    attemp_lan_join();
}