#ifndef UI_LISTS_H
#define UI_LISTS_H


#include "../../engine/engine.h"
#include "billboards.h"
#include "nine_slices.h"
#include "fonts.h"


#define _MAX_UI_LISTS_AMOUNT_ (12)
#define _UI_LIST_MAX_ELEMENTS_AMOUNT_ (8)


/* param 1: the ui_list id
 * param 2: the button id
 */
typedef void(*ui_list_element_callback_t)(int32_t, int32_t);

typedef enum {
    ELEMENT_TYPE_BUTTON,
    ELEMENT_TYPE_SLIDER
} UI_LIST_ELEMENT_TYPE;

typedef struct {
    UI_LIST_ELEMENT_TYPE type;
    union {
        struct {
            ui_list_element_callback_t callback;
            char* string;
            font_t* font;
            int32_t padding;
            nine_slice_t* nslice;
            nine_slice_t* hover_nslice;

            char* info_string;
            font_t* info_string_font;
            int32_t info_string_padding;
            nine_slice_t* info_string_nslice;
        } button;
        struct {
            float* value;
            ui_list_element_callback_t callback;
            int32_t width;
            int32_t height;
            int32_t padding;
            nine_slice_t* nslice;
            int32_t pin_width;
            int32_t pin_height;
            nine_slice_t* pin_nslice;
            nine_slice_t* full_nslice;
            nine_slice_t* empty_nslice;
            
            char* info_string;
            font_t* info_string_font;
            int32_t info_string_padding;
            nine_slice_t* info_string_nslice;
        } slider;
    };
} ui_list_element_t;

typedef struct {
    uint8_t active : 1;
    uint8_t safe : 1;
    uint8_t permenant : 1;

    uint8_t hidden : 1;

    billboard_t billboard;
    int32_t padding;
    nine_slice_t* nslice;

    uint32_t elements_amount;
    ui_list_element_t elements[_UI_LIST_MAX_ELEMENTS_AMOUNT_];

    int32_t parent_ui_list;
    int32_t child_ui_list;
} ui_list_t;


extern ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];


int32_t new_ui_list_assign_id();
int32_t new_ui_list(int32_t x, int32_t y, int32_t padding, nine_slice_t* nine_slice);
int32_t new_ui_list_billboarded(int32_t world_pos_x, int32_t world_pos_y, int32_t world_pos_z, camera_t* billboard_camera,
                                int32_t offset_x, int32_t offset_y,
                                int32_t padding, nine_slice_t* nine_slice);
int32_t new_ui_list_child(int32_t parent_id, int32_t offset_x, int32_t offset_y, int32_t padding, nine_slice_t* nine_slice);

void ui_list_add_element(int32_t id, ui_list_element_t element);

void set_ui_lists_to_unsafe();
void close_ui_list(int32_t id);
void make_ui_list_safe(int32_t id);
void make_ui_list_permenant(int32_t id);
void make_ui_list_hidden(int32_t id);
void close_unsafe_ui_lists();
void close_unperm_ui_lists();
void close_all_ui_lists();

uint32_t get_ui_list_slider_element_width(ui_list_element_t* slider_element);
uint32_t get_ui_list_slider_element_height(ui_list_element_t* slider_element);
uint32_t get_ui_list_button_element_width(ui_list_element_t* button_element);
uint32_t get_ui_list_button_element_height(ui_list_element_t* button_element);
uint32_t get_ui_list_element_width(int32_t id, int32_t element_i);
uint32_t get_ui_list_element_height(int32_t id, int32_t element_i);
void update_ui_list_billboard_size(int32_t id);
int32_t get_ui_list_width(int32_t id);
int32_t get_ui_list_height(int32_t id);
ivec2_t get_ui_list_element_padded_pos(int32_t id, int32_t ei);
ivec2_t get_ui_list_pos(int32_t id);
ivec2_t get_ui_list_pos_padded(int32_t id);
/* x = x pos inside ui-list
 * y = y pos inside ui-list
 * z = ui_list-index
 * or `-1` in everything if not inside ui_list
 */
ivec3_t get_ui_list_inside_pos();

// TODO: is anybody even using this???
ivec2_t get_ui_list_button_element_info_size(int32_t id, int32_t element_i, char* info_str);

/* x = ui_list_id
 * y = element_i
 */
ivec2_t get_ui_list_hovered_element();

void ui_list_pressed_slider_element_update();
void ui_lists_update();

void ui_list_slider_element_pressed(int32_t id, int32_t ei);
void ui_list_button_element_pressed(int32_t id, int32_t ei);
void ui_lists_handle_mouse_pressed();

void ui_lists_handle_mouse_released();

void draw_ui_list_hovered_element_info_string();
void draw_ui_list_slider_element(int32_t id, int32_t element_i, int8_t hovered);
void draw_ui_list_button_element(int32_t id, int32_t element_i, int8_t hovered);
void draw_ui_list_element(int32_t id, int32_t element_i, int8_t hovered);
void draw_ui_list(int32_t id);
void draw_all_ui_lists();

#endif