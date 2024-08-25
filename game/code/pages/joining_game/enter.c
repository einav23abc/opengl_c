#include "page.h"
#include "../in_game/against_connected.h"
#include "../../game.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);

void enter_joining_game() {
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
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
        #ifdef DEBUG_SOFT_MODE
        printf("exiting joining game - closing server\n");
        #endif
        close_server();
    }
    if (play_type == PLAY_TYPE_AGAINST_HOST) {
        #ifdef DEBUG_SOFT_MODE
        printf("exiting joining game - exiting server\n");
        #endif
        exit_server();
    }
    switch_page(PAGE_MAIN_MENU);
}