#include "page.h"
#include "../in_game/page.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);
static void play_against_ai_button_callback(int32_t ui_list_id, int32_t button_id);
static void open_lan_button_callback(int32_t ui_list_id, int32_t button_id);
static void join_lan_button_callback(int32_t ui_list_id, int32_t button_id);

void enter_play() {
    close_all_ui_lists();
    close_all_alerts();
    
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

    // against ai
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
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("Play against AIs")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 + big_letters_font.letter_height*2,

        .buttons_amount = 1,
        .button_strings = {"Play against AIs"},
        .button_info_strings = {"Play against varying enemy AIs"},
        .button_callbacks = {&play_against_ai_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };

    // open LAN
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
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("Open LAN game")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4,

        .buttons_amount = 1,
        .button_strings = {"Open LAN game"},
        .button_info_strings = {"Play against a friend on LAN"},
        .button_callbacks = {&open_lan_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };

    // join LAN
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
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("Join LAN game")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 - big_letters_font.letter_height*2,

        .buttons_amount = 1,
        .button_strings = {"Join LAN game"},
        .button_info_strings = {"Play against a friend on LAN"},
        .button_callbacks = {&join_lan_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_MAIN_MENU);
}
static void play_against_ai_button_callback(int32_t ui_list_id, int32_t button_id) {
    play_type = PLAY_TYPE_AGAINST_AI;
    switch_page(PAGE_IN_GAME);
}
static void open_lan_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_OPEN_LAN);
}
static void join_lan_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_JOIN_LAN);
}