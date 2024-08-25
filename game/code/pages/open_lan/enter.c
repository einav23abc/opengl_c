#include "page.h"
#include "../in_game/against_connected.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

char open_lan_waiting_for_connection_string[60];

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);

void enter_open_lan() {
    close_all_ui_lists();
    close_all_alerts();

    if (open_server_local() != 0) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open local server\n");
        #endif
        switch_page(PAGE_MAIN_MENU);
        add_big_error_alert("Failed to open server locally.");
        return;
    }

    // client id of server is `-1`
    my_client_id = -1;
    play_type = PLAY_TYPE_AGAINST_CLIENT;
    
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
    close_server();
    switch_page(PAGE_MAIN_MENU);
}