#include "page.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void how_to_play_button_callback(int32_t ui_list_id, int32_t button_id);
static void play_button_callback(int32_t ui_list_id, int32_t button_id);


void enter_main_menu() {
    close_all_ui_lists();
    close_all_alerts();
    
    int32_t ui_list_id;

    // set up ui lists
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
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("Play")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 + big_letters_font.letter_height,

        .buttons_amount = 1,
        .button_strings = {"Play"},
        .button_info_strings = {""},
        .button_callbacks = {&play_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    
    // set up ui lists
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
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("How to play")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 - big_letters_font.letter_height,

        .buttons_amount = 1,
        .button_strings = {"How to play"},
        .button_info_strings = {""},
        .button_callbacks = {&how_to_play_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}

static void how_to_play_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_HOW_TO_PLAY);
}
static void play_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_PLAY);
}