#include "page.h"
#include "../in_game/against_connected.h"
#include "../../alerts.h"

void attemp_lan_join() {
    if (ip_input_current_byte < 3 || ip_input_byte_current_number[3] == 0) {
        add_error_alert_at_cursor("Invalid ip address");
        return;
    }

    int32_t join_res = join_server(ip_input_string);
    if (join_res == -1) {
        add_error_alert_at_cursor("Failed to join");
        return;
    }
    my_client_id = join_res;
    
    switch_page(PAGE_JOINING_GAME);
    
    // will switch to the in-game page from `parse_state_packet`
}