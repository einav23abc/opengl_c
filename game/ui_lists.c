#include "ui_lists.h"

ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];


int32_t new_ui_list_assign_id() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 0) return i;
    }
}
void set_ui_lists_to_unsafe() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].safe = 0;
}
void close_ui_list(int32_t i) {
    if (i < 0 || i >= _MAX_UI_LISTS_AMOUNT_) return;
    ui_lists[i].active = 0;
    if (ui_lists[i].child_ui_list >= 0) close_ui_list(ui_lists[i].child_ui_list);
}
void make_ui_list_safe(int32_t i) {
    if (i < 0 || i >= _MAX_UI_LISTS_AMOUNT_) return;
    ui_lists[i].safe = 1;
    if (ui_lists[i].parent_ui_list >= 0) make_ui_list_safe(ui_lists[i].parent_ui_list);
}
void close_unsafe_ui_lists() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].permenant == 1) continue;
        if (ui_lists[i].safe == 0) ui_lists[i].active = 0;
    }
}
void close_unperm_ui_lists() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].permenant == 0) ui_lists[i].active = 0;
    }
}
void close_all_ui_lists() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        ui_lists[i].active = 0;
    }
}
uint32_t get_ui_list_width(int32_t i) {
    uint32_t button_string_max_length = 1;
    for (uint32_t j = 0; j < ui_lists[i].buttons_amount; j++) {
        button_string_max_length = max(button_string_max_length, strlen(ui_lists[i].button_strings[j]));
    }
    return (button_string_max_length * letters_font.letter_width*(_UI_LIST_BUTTON_HEIGHT_/letters_font.letter_height));
}
uint32_t get_ui_list_height(int32_t i) {
    return (_UI_LIST_BUTTON_HEIGHT_*ui_lists[i].buttons_amount);
}
uvec2_t get_ui_list_box_pos(int32_t i) {
    float x = 0;
    float y = 0;
    if (ui_lists[i].box_pos_from_world_pos == 1) {
        vec2_t screen_cord = outport_space_position_from_world_space(vec3(ui_lists[i].box_world_pos_x, ui_lists[i].box_world_pos_y, ui_lists[i].box_world_pos_z));
        x = screen_cord.x;
        y = screen_cord.y;
    }

    x += ui_lists[i].x;
    y += ui_lists[i].y;

    y -= get_ui_list_height(i);

    return (uvec2_t){
        .x = x,
        .y = y
    };
}
uvec2_t get_ui_list_box_pos_padded(int32_t i) {
    uvec2_t box_pos = get_ui_list_box_pos(i);
    box_pos.x -= _UI_LIST_PADDING_;
    box_pos.y -= _UI_LIST_PADDING_;
    return box_pos;
}
ivec3_t get_ui_list_inside_pos() {
    vec2_t outport_space_position = get_mouse_outport_space_position();
    
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        uvec2_t box_pos = get_ui_list_box_pos(i);
        if (
            ui_lists[i].active == 1 &&
            box_pos.x <= outport_space_position.x &&
            box_pos.y <= outport_space_position.y &&
            box_pos.x + get_ui_list_width(i)  >= outport_space_position.x &&
            box_pos.y + get_ui_list_height(i) >= outport_space_position.y
        ) {
            return (ivec3_t){
                .x = outport_space_position.x - box_pos.x,
                .y = outport_space_position.y - box_pos.y,
                .z = i
            };
        }
    }
    return (ivec3_t){
        .x = -1,
        .y = -1,
        .z = -1
    };
}
uvec2_t get_ui_button_info_size(char* info_str) {
    return get_str_boxed_size(info_str, _UI_LIST_BUTTON_INFO_ROW_HEIGHT_);
}