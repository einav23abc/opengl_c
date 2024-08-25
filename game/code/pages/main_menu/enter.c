#include "page.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void how_to_play_button_callback(int32_t ui_list_id, int32_t button_id);
static void play_button_callback(int32_t ui_list_id, int32_t button_id);


void enter_main_menu() {
    close_all_ui_lists();
    close_all_alerts();
    
    int32_t ui_list_id;

    // play
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5  - (strlen("Play")*big_letters_font.letter_width + 4*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 + big_letters_font.letter_height,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &play_button_callback,
                .string = "Play",
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
    
    // how to play
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5  - (strlen("How to play")*big_letters_font.letter_width + 4*2)*0.5,
        _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 - big_letters_font.letter_height,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &how_to_play_button_callback,
                .string = "How to play",
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

static void how_to_play_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_HOW_TO_PLAY);
}
static void play_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_PLAY);
}