#ifndef UI_LISTS_H
#define UI_LISTS_H

#include "game.h"


#define _MAX_UI_LISTS_AMOUNT_ (4)
#define _UI_LIST_MAX_BUTTONS_AMOUNT_ (8)
#define _UI_LIST_PADDING_ (3)
#define _UI_LIST_BUTTON_HEIGHT_ (12)
#define _UI_LIST_BUTTON_INFO_ROW_HEIGHT_ (12)


/* param 1: the ui_list id
 * param 2: the button id
 */
typedef void(*button_callback_t)(int32_t, int32_t);

typedef struct {
    uint8_t active : 1;

    uint8_t box_pos_from_world_pos : 1;
    float box_world_pos_x;
    float box_world_pos_y;
    float box_world_pos_z;
    // if box_pos_from_world_pos = 1, this is a screen-x translation
    int32_t x;
    // if box_pos_from_world_pos = 1, this is a screen-y translation
    int32_t y;

    uint32_t buttons_amount;
    char* button_strings[_UI_LIST_MAX_BUTTONS_AMOUNT_];
    char* button_info_strings[_UI_LIST_MAX_BUTTONS_AMOUNT_];
    button_callback_t button_callbacks[_UI_LIST_MAX_BUTTONS_AMOUNT_];

    uint8_t safe : 1;
    int32_t parent_ui_list;
    int32_t child_ui_list;
} ui_list_t;


extern ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];


int32_t new_ui_list_assign_id();
void set_ui_lists_to_unsafe();
void close_ui_list(int32_t i);
void make_ui_list_safe(int32_t i);
void close_unsafe_ui_lists();
void close_all_ui_lists();
uint32_t get_ui_list_width(int32_t i);
uint32_t get_ui_list_height(int32_t i);
uvec2_t get_ui_list_box_pos(int32_t i);
uvec2_t get_ui_list_box_pos_padded(int32_t i);
/* x = x pos inside box
 * y = y pos inside box
 * z = ui_list-index
 * or `-1` in everything if not inside ui_list
 */
ivec3_t get_ui_list_inside_pos();
uvec2_t get_ui_button_info_size(char* info_str);


#endif