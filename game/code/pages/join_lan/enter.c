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

    // join
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5 + (strlen("XXX.XXX.XXX.XXX")*big_letters_font.letter_width + 5*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &join_button_callback,
                .string = "Join",
                .font = &big_letters_font,
                .padding = 4,
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
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_MAIN_MENU);
}
static void join_button_callback(int32_t ui_list_id, int32_t button_id) {
    attemp_lan_join();
}