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
    return (button_string_max_length * ui_lists[i].font->letter_width + ui_lists[i].button_padding*2);
}
uint32_t get_ui_list_height(int32_t i) {
    return ((ui_lists[i].font->letter_height + ui_lists[i].button_padding*2) * ui_lists[i].buttons_amount);
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
    box_pos.x -= ui_lists[i].padding;
    box_pos.y -= ui_lists[i].padding;
    return box_pos;
}
ivec3_t get_ui_list_inside_pos() {
    vec2_t outport_space_position = get_mouse_outport_space_position();
    
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 0) continue;

        uvec2_t box_pos = get_ui_list_box_pos(i);
        if (
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
ivec2_t get_ui_list_hovered_button() {
    ivec3_t cursor_inside_box_pos = get_ui_list_inside_pos();
    if (cursor_inside_box_pos.z != -1) {
        int32_t i = cursor_inside_box_pos.z;

        int32_t cursor_inside_button = floor(((float)cursor_inside_box_pos.y)/(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2));

        if (cursor_inside_button >= 0 && cursor_inside_button < ui_lists[i].buttons_amount) {
            return (ivec2_t){
                .x = i,
                .y = cursor_inside_button
            };
        }
    }
    return (ivec2_t){
        .x = -1,
        .y = -1
    };
}
uvec2_t get_ui_list_button_info_size(int32_t i, char* info_str) {
    return get_str_boxed_size(info_str, ui_lists[i].font->letter_height);
}
void draw_ui_list(int32_t i) {
    uvec2_t box_pos = get_ui_list_box_pos(i);
    uvec2_t box_padded_pos = (uvec2_t){
        box_pos.x - ui_lists[i].padding,
        box_pos.y - ui_lists[i].padding,
    };

    uint32_t box_width = get_ui_list_width(i);
    uint32_t box_height = get_ui_list_height(i);

    ivec3_t cursor_inside_box_pos = get_ui_list_inside_pos();
    int32_t cursor_inside_button = -1;
    if (cursor_inside_box_pos.z == i) {
        cursor_inside_button = floor(((float)cursor_inside_box_pos.y)/(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2));
    }

    sdm_set_color(1,0,0,1);
    sdm_draw_rect(box_padded_pos.x, box_padded_pos.y, box_width+ui_lists[i].padding*2, box_height+ui_lists[i].padding*2);
    
    // buttons
    for (int32_t j = 0; j < ui_lists[i].buttons_amount; j++) {
        // hovering indication
        if (cursor_inside_button == j && ui_lists[i].button_callbacks[j] != NULL) {
            sdm_set_color(0.5,0.5,0,1);
            sdm_draw_rect(box_pos.x, box_pos.y + j*(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2), box_width, (ui_lists[i].font->letter_height + ui_lists[i].button_padding*2));
        }

        draw_string(
            *(ui_lists[i].font),
            ui_lists[i].button_strings[j],
            (vec3_t){
                .x = box_pos.x + ui_lists[i].button_padding,
                .y = box_pos.y + ui_lists[i].button_padding + j*(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2),
                .z = 0
            },
            quat_from_axis_angles_yzx(-0, -0, -0),
            (ui_lists[i].font->letter_height),
            1, 1, 1
        );
    }
}
void draw_ui_list_hovered_button_info_string() {
    ivec3_t cursor_inside_box_pos = get_ui_list_inside_pos();
    if (cursor_inside_box_pos.z != -1) {
        int32_t i = cursor_inside_box_pos.z;

        uvec2_t box_pos = get_ui_list_box_pos(i);
        int32_t cursor_inside_button = floor(((float)cursor_inside_box_pos.y)/(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2));

        if (cursor_inside_button >= 0 && cursor_inside_button < ui_lists[i].buttons_amount) {
            if (ui_lists[i].button_info_strings[cursor_inside_button][0] != '\0') {
                uvec2_t box_size = get_str_boxed_size(ui_lists[i].button_info_strings[cursor_inside_button], (ui_lists[i].font->letter_height));

                uint32_t info_str_box_padding = ui_lists[i].padding;

                uint32_t left_x = box_pos.x + cursor_inside_box_pos.x + info_str_box_padding;
                uint32_t bottom_y = box_pos.y + (cursor_inside_button + 1)*(ui_lists[i].font->letter_height + ui_lists[i].button_padding*2) + info_str_box_padding;

                if (left_x - info_str_box_padding <= 0) {
                    left_x = info_str_box_padding;
                }
                if (left_x + box_size.x + info_str_box_padding >= _OUTPORT_WIDTH_) {
                    left_x = _OUTPORT_WIDTH_ - info_str_box_padding - box_size.x;
                }
                if (bottom_y - info_str_box_padding <= 0) {
                    bottom_y = info_str_box_padding;
                }
                if (bottom_y + box_size.y + info_str_box_padding >= _OUTPORT_HEIGHT_) {
                    bottom_y = _OUTPORT_HEIGHT_ - info_str_box_padding - box_size.y;
                }

                draw_str_boxed(ui_lists[i].button_info_strings[cursor_inside_button], letters_font, left_x, bottom_y, info_str_box_padding, (ui_lists[i].font->letter_height));
            }
        }
    }
}