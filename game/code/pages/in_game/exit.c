#include "page.h"

void exit_in_game() {
    if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
        close_server();
    }
    if (play_type == PLAY_TYPE_AGAINST_HOST) {
        exit_server();
    }
}