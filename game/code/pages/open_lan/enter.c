#include "page.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

char open_lan_waiting_for_connection_string[60];

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);

void enter_open_lan() {
    close_all_ui_lists();
    close_all_alerts();
    
    int32_t ui_list_id;

    if (open_server_local() != 0) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open local server\n");
        #endif
        switch_page(PAGE_MAIN_MENU);
        add_error_alert_at_cursor("Failed to open server locally.");
        return;
    }
    
    // <generate waiting for connetction message>
        char ip_string[16];
        int32_t ip_string_length = ipstring_from_ipuint(ip_string, get_server_ip());

        int32_t c = 0;
        // spaces 1
        for (int32_t space_i = 0; space_i < ((12+ip_string_length)-25)>>1; space_i++) {
            open_lan_waiting_for_connection_string[c] = ' ';
            c += 1;
        }
        strcpy(&(open_lan_waiting_for_connection_string[c]), "Waiting for a connection.\n");
        c += strlen("Waiting for a connection.\n");
        // spaces 2
        for (int32_t space_i = 0; space_i < (25-(12+ip_string_length))>>1; space_i++) {
            open_lan_waiting_for_connection_string[c] = ' ';
            c += 1;
        }
        strcpy(&(open_lan_waiting_for_connection_string[c]), "Connect at: ");
        c += strlen("Connect at: ");
        strcpy(&(open_lan_waiting_for_connection_string[c]), ip_string);
        c += strlen(ip_string);
        open_lan_waiting_for_connection_string[c] = '\0';
    // </generate waiting for connetction message>

    
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
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    close_server();
    switch_page(PAGE_MAIN_MENU);
}