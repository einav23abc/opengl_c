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
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - (strlen("Main menu")*letters_font.letter_width + 5*2) - 1,
        _OUTPORT_HEIGHT_ - 1,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &main_menu_button_callback,
                .string = "Main menu",
                .font = &letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    make_ui_list_permenant(ui_list_id);

    // against ai
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5  - (strlen("Play against AIs")*big_letters_font.letter_width + 4*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 + big_letters_font.letter_height*2,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &play_against_ai_button_callback,
                .string = "Play against AIs",
                .font = &big_letters_font,
                .padding = 4,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Play against varying enemy AIs",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    make_ui_list_permenant(ui_list_id);


    // open LAN
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5  - (strlen("Open LAN game")*big_letters_font.letter_width + 4*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &open_lan_button_callback,
                .string = "Open LAN game",
                .font = &big_letters_font,
                .padding = 4,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Play against a friend on LAN",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    make_ui_list_permenant(ui_list_id);

    // join LAN
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5  - (strlen("Join LAN game")*big_letters_font.letter_width + 4*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 - big_letters_font.letter_height*2,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &join_lan_button_callback,
                .string = "Join LAN game",
                .font = &big_letters_font,
                .padding = 4,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Play against a friend on LAN",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    make_ui_list_permenant(ui_list_id);
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