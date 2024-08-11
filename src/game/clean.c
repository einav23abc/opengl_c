#include "game.h"

void clean() {
    if (is_server_host) {
        close_server();
    }else {
        exit_server();
    }
    return;
}