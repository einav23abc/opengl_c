#ifndef PAGE_JOIN_LAN_H
#define PAGE_JOIN_LAN_H

#include "../../game.h"

// declared in "join_lan/key_press.c"
extern char ip_input_string[16];
// declared in "join_lan/key_press.c"
extern int8_t ip_input_current_char;
// declared in "join_lan/key_press.c"
extern int8_t ip_input_current_byte;
// declared in "join_lan/key_press.c"
extern int8_t ip_input_byte_current_number[4];

void enter_join_lan();
void render_join_lan();
void key_press_join_lan();

#endif