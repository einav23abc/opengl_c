#ifndef PAGE_OPEN_LAN_H
#define PAGE_OPEN_LAN_H

#include "../../game.h"

// declared in "open_lan/enter.c"
extern char open_lan_waiting_for_connection_string[60];

void enter_open_lan();
void render_open_lan();



#endif