#include "ui_lists.h"
#include "game.h"
#include "sounds.h"
#include "spaces.h"

ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];
static ivec2_t pressed_slider = (ivec2_t){-1, -1};


int32_t new_ui_list_assign_id() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 0) return i;
    }
    return -1;
}
int32_t new_ui_list(int32_t x, int32_t y, int32_t padding, nine_slice_t* nine_slice) {
    int32_t id = new_ui_list_assign_id();
    if (id == -1) return -1;

    ui_lists[id] = (ui_list_t) {
        .active = 1,
        .safe = 0,
        .permenant = 0,

        .hidden = 0,

        .billboard = (billboard_t) {
            .box_pos_from_world_pos = 0,
            .x = x,
            .y = y
        },
        .padding = padding,
        .nslice = nine_slice,

        .elements_amount = 0,

        .parent_ui_list = -1,
        .child_ui_list = -1
    };
    
    update_ui_list_billboard_size(id);

    return id;
}
int32_t new_ui_list_billboarded(int32_t world_pos_x, int32_t world_pos_y, int32_t world_pos_z, camera_t* billboard_camera,
                                int32_t offset_x, int32_t offset_y,
                                int32_t padding, nine_slice_t* nine_slice) {
    int32_t id = new_ui_list_assign_id();
    if (id == -1) return -1;

    ui_lists[id] = (ui_list_t) {
        .active = 1,
        .safe = 0,
        .permenant = 0,

        .hidden = 0,

        .billboard = (billboard_t) {
            .box_pos_from_world_pos = 1,
            .box_world_pos_x = world_pos_x,
            .box_world_pos_y = world_pos_y,
            .box_world_pos_z = world_pos_z,
            .box_world_pos_camera = billboard_camera,

            .x = offset_x,
            .y = offset_y
        },
        .padding = padding,
        .nslice = nine_slice,

        .elements_amount = 0,

        .parent_ui_list = -1,
        .child_ui_list = -1
    };
    
    update_ui_list_billboard_size(id);

    return id;
}
int32_t new_ui_list_child(int32_t parent_id, int32_t offset_x, int32_t offset_y, int32_t padding, nine_slice_t* nine_slice) {
    int32_t id = new_ui_list_assign_id();
    if (id == -1) return -1;

    if (ui_lists[parent_id].child_ui_list != -1) {
        ui_lists[ui_lists[parent_id].child_ui_list].parent_ui_list = -1;
    }

    ui_lists[parent_id].child_ui_list = id;

    ui_lists[id] = (ui_list_t) {
        .active = 1,
        .safe = 0,
        .permenant = 0,

        .hidden = 0,

        .billboard = (billboard_t) {
            .box_pos_from_world_pos = ui_lists[parent_id].billboard.box_pos_from_world_pos,
            .box_world_pos_x = ui_lists[parent_id].billboard.box_world_pos_x,
            .box_world_pos_y = ui_lists[parent_id].billboard.box_world_pos_y,
            .box_world_pos_z = ui_lists[parent_id].billboard.box_world_pos_z,
            .box_world_pos_camera = ui_lists[parent_id].billboard.box_world_pos_camera,
            .x = ui_lists[parent_id].billboard.x + offset_x,
            .y = ui_lists[parent_id].billboard.y + offset_y
        },
        .padding = padding,
        .nslice = nine_slice,

        .elements_amount = 0,

        .parent_ui_list = parent_id,
        .child_ui_list = -1
    };

    update_ui_list_billboard_size(id);
    
    return id;
}

void ui_list_add_element(int32_t id, ui_list_element_t element) {
    if (ui_lists[id].elements_amount == _UI_LIST_MAX_ELEMENTS_AMOUNT_) return;
    
    ui_lists[id].elements[ui_lists[id].elements_amount] = element;
    ui_lists[id].elements_amount += 1;

    update_ui_list_billboard_size(id);
}

void set_ui_lists_to_unsafe() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].safe = 0;
}
void close_ui_list(int32_t id) {
    if (id < 0 || id >= _MAX_UI_LISTS_AMOUNT_) return;
    ui_lists[id].active = 0;
    if (ui_lists[id].child_ui_list >= 0) close_ui_list(ui_lists[id].child_ui_list);
    if (ui_lists[id].parent_ui_list >= 0) ui_lists[ui_lists[id].parent_ui_list].child_ui_list = -1;
}
void make_ui_list_safe(int32_t id) {
    if (id < 0 || id >= _MAX_UI_LISTS_AMOUNT_) return;
    ui_lists[id].safe = 1;
    if (ui_lists[id].parent_ui_list >= 0) make_ui_list_safe(ui_lists[id].parent_ui_list);
}
void make_ui_list_permenant(int32_t id) {
    ui_lists[id].permenant = 1;
}
void make_ui_list_hidden(int32_t id) {
    ui_lists[id].hidden = 1;
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

uint32_t get_ui_list_slider_element_width(ui_list_element_t* slider_element) {
    return (slider_element->slider.width + slider_element->slider.padding*2);
}
uint32_t get_ui_list_slider_element_height(ui_list_element_t* slider_element) {
    return (slider_element->slider.height + slider_element->slider.padding*2);
}
uint32_t get_ui_list_button_element_width(ui_list_element_t* button_element) {
    return (strlen(button_element->button.string) * button_element->button.font->letter_width + button_element->button.padding*2);
}
uint32_t get_ui_list_button_element_height(ui_list_element_t* button_element) {
    return (button_element->button.font->letter_height + button_element->button.padding*2);
}
uint32_t get_ui_list_element_width(int32_t id, int32_t element_i) {
    switch (ui_lists[id].elements[element_i].type) {
        case ELEMENT_TYPE_BUTTON: {
            return get_ui_list_button_element_width(&(ui_lists[id].elements[element_i]));
            break;
        }
        case ELEMENT_TYPE_SLIDER: {
            return get_ui_list_slider_element_width(&(ui_lists[id].elements[element_i]));
            break;
        }
    }
}
uint32_t get_ui_list_element_height(int32_t id, int32_t element_i) {
    switch (ui_lists[id].elements[element_i].type) {
        case ELEMENT_TYPE_BUTTON: {
            return get_ui_list_button_element_height(&(ui_lists[id].elements[element_i]));
            break;
        }
        case ELEMENT_TYPE_SLIDER: {
            return get_ui_list_slider_element_height(&(ui_lists[id].elements[element_i]));
            break;
        }
    }
}
void update_ui_list_billboard_size(int32_t id) {
    int32_t max_element_width = 0;
    for (uint32_t ei = 0; ei < ui_lists[id].elements_amount; ei++) {
        max_element_width = max(max_element_width, get_ui_list_element_width(id, ei));
    }
    ui_lists[id].billboard.box_width = max_element_width;

    int32_t height = 0;
    for (uint32_t ei = 0; ei < ui_lists[id].elements_amount; ei++) {
        height += get_ui_list_element_height(id, ei);
    }
    ui_lists[id].billboard.box_height = height;
}
int32_t get_ui_list_width(int32_t id) {
    return (ui_lists[id].billboard.box_width);
}
int32_t get_ui_list_height(int32_t id) {
    return (ui_lists[id].billboard.box_height);
}
ivec2_t get_ui_list_element_padded_pos(int32_t id, int32_t element_i) {
    ivec2_t pos = get_ui_list_pos(id);
    for (uint32_t ei = 0; ei < element_i; ei++) {
        pos.y += get_ui_list_element_height(id, ei);
    }
    return pos;
}
ivec2_t get_ui_list_pos(int32_t id) {
    return get_billboard_box_pos(ui_lists[id].billboard);
}
ivec2_t get_ui_list_pos_padded(int32_t id) {
    ivec2_t pos = get_ui_list_pos(id);
    pos.x -= ui_lists[id].padding;
    pos.y -= ui_lists[id].padding;
    return pos;
}
ivec3_t get_ui_list_inside_pos() {
    vec2_t foutport_space_position = fbo_view_position_from_mouse_position(outport_fbo);
    ivec2_t outport_space_position = (ivec2_t){
        .x = (int32_t)foutport_space_position.x,
        .y = (int32_t)foutport_space_position.y
    };
    
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 0) continue;

        ivec2_t pos = get_ui_list_pos(i);
        if (
            pos.x <= outport_space_position.x &&
            pos.y <= outport_space_position.y &&
            pos.x + get_ui_list_width(i)  >= outport_space_position.x &&
            pos.y + get_ui_list_height(i) >= outport_space_position.y
        ) {
            return (ivec3_t){
                .x = outport_space_position.x - pos.x,
                .y = outport_space_position.y - pos.y,
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

ivec2_t get_ui_list_button_element_info_size(int32_t id, int32_t element_i, char* info_str) {
    return get_str_boxed_size(ui_lists[id].elements[element_i].button.info_string_font, info_str, ui_lists[id].elements[element_i].button.info_string_font->letter_height);
}

ivec2_t get_ui_list_hovered_element() {
    ivec3_t in_pos = get_ui_list_inside_pos();

    int32_t id = in_pos.z;
    if (id == -1) {
        return (ivec2_t){
            .x = -1,
            .y = -1
        };
    }

    int32_t y = 0;
    for (int32_t ei = 0; ei < ui_lists[id].elements_amount; ei++) {
        int32_t width  = get_ui_list_element_width(id, ei);
        int32_t height = get_ui_list_element_height(id, ei);
        if (
            in_pos.x <= width &&
            in_pos.y <= y + height &&
            in_pos.y >= y
        ) {
            return (ivec2_t) {
                .x = id,
                .y = ei
            };
        }
        y += height;
    }

    return (ivec2_t){
        .x = -1,
        .y = -1
    };
}


void ui_list_pressed_slider_element_update() {
    if (pressed_slider.x == -1 || pressed_slider.y == -1) return;
    
    
    if (pressed_slider.x >= _MAX_UI_LISTS_AMOUNT_) {
        pressed_slider = (ivec2_t){-1, -1};
        return;
    }
    if (pressed_slider.y >= ui_lists[pressed_slider.x].elements_amount) {
        pressed_slider = (ivec2_t){-1, -1};
        return;
    }
    if (ui_lists[pressed_slider.x].active == 0) {
        pressed_slider = (ivec2_t){-1, -1};
        return;
    }
    
    ui_list_element_t* element = &(ui_lists[pressed_slider.x].elements[pressed_slider.y]);

    if (element->type != ELEMENT_TYPE_SLIDER) {
        pressed_slider = (ivec2_t){-1, -1};
        return;
    }

    int32_t pin_x = fbo_view_position_from_mouse_position(outport_fbo).x - get_billboard_box_pos(ui_lists[pressed_slider.x].billboard).x - element->slider.pin_width;
    float value = ((float)pin_x)/((float)element->slider.width - element->slider.pin_width);

    value = max(0.0, min(1.0, value));
    
    *(element->slider.value) = value;

    ui_list_element_callback_t callback = element->slider.callback;
    if (callback != NULL)
        callback(pressed_slider.x, pressed_slider.y);
}
void ui_lists_update() {
    ui_list_pressed_slider_element_update();
}

void ui_list_slider_element_pressed(int32_t id, int32_t ei) {
    pressed_slider = (ivec2_t){id, ei};
    ui_list_pressed_slider_element_update();
}
void ui_list_button_element_pressed(int32_t id, int32_t ei) {
    audio_sound_play(button_press_sound);

    ui_list_element_callback_t button_callback = ui_lists[id].elements[ei].button.callback;
    if (button_callback != NULL){
        button_callback(id, ei);
    }
}
void ui_lists_handle_mouse_pressed() {
    set_ui_lists_to_unsafe();

    // reset pressed slider
    pressed_slider = (ivec2_t){-1, -1};
    
    ivec2_t hovered_element = get_ui_list_hovered_element();

    if (hovered_element.x == -1 || hovered_element.y == -1) {
        close_unsafe_ui_lists();
        return;
    }
    
    make_ui_list_safe(hovered_element.x);

    switch (ui_lists[hovered_element.x].elements[hovered_element.y].type) {
        case ELEMENT_TYPE_BUTTON: {
            ui_list_button_element_pressed(hovered_element.x, hovered_element.y);
            break;
        }
        case ELEMENT_TYPE_SLIDER: {
            ui_list_slider_element_pressed(hovered_element.x, hovered_element.y);
            break;
        }
    }

    close_unsafe_ui_lists();
}

void ui_lists_handle_mouse_released() {
    // reset pressed slider
    pressed_slider = (ivec2_t){-1, -1};
}

void draw_ui_list_hovered_element_info_string() {
    ivec2_t in_element = get_ui_list_hovered_element();
    if (in_element.x == -1 || in_element.y == -1) return;

    vec2_t outport_space_position = fbo_view_position_from_mouse_position(outport_fbo);
    ivec2_t pos = get_ui_list_element_padded_pos(in_element.x, in_element.y);

    char* str;
    font_t font;
    nine_slice_t nine_slice;
    int32_t padding;

    switch (ui_lists[in_element.x].elements[in_element.y].type) {
        case ELEMENT_TYPE_BUTTON: {
            str = ui_lists[in_element.x].elements[in_element.y].button.info_string;
            if (str == NULL) return;
            if (str[0] == '\0') return;
            font = *(ui_lists[in_element.x].elements[in_element.y].button.info_string_font);
            nine_slice = *(ui_lists[in_element.x].elements[in_element.y].button.info_string_nslice);
            padding = ui_lists[in_element.x].elements[in_element.y].button.info_string_padding;
            break;
        }
        case ELEMENT_TYPE_SLIDER: {
            str = ui_lists[in_element.x].elements[in_element.y].slider.info_string;
            if (str == NULL) return;
            if (str[0] == '\0') return;
            font = *(ui_lists[in_element.x].elements[in_element.y].slider.info_string_font);
            nine_slice = *(ui_lists[in_element.x].elements[in_element.y].slider.info_string_nslice);
            padding = ui_lists[in_element.x].elements[in_element.y].slider.info_string_padding;
            break;
        }
    }

    int32_t left_x = outport_space_position.x;
    int32_t bottom_y = pos.y + get_ui_list_element_height(in_element.x, in_element.y) + padding;
    
    ivec2_t str_box_size = get_str_boxed_size(&font, str, font.letter_height);

    if (left_x - padding < 0)
        left_x = padding;
    if (left_x + padding + str_box_size.x > _OUTPORT_WIDTH_)
        left_x = _OUTPORT_WIDTH_ - str_box_size.x - padding;
    if (bottom_y - padding < 0)
        bottom_y = padding;
    if (bottom_y + padding + str_box_size.y > _OUTPORT_HEIGHT_)
        bottom_y = _OUTPORT_HEIGHT_ - str_box_size.y - padding;

    draw_str_boxed(
        str,
        font,
        nine_slice,
        left_x,
        bottom_y,
        padding,
        font.letter_height
    );
}
void draw_ui_list_slider_element(int32_t id, int32_t element_i, int8_t hovered) {
    ivec2_t padded_pos = get_ui_list_element_padded_pos(id, element_i);
    int32_t width = get_ui_list_element_width(id, element_i);
    int32_t height = get_ui_list_element_height(id, element_i);

    ui_list_element_t* element = &(ui_lists[id].elements[element_i]);
    
    if (element->slider.nslice != NULL) {
        draw_nine_slice(
            *(element->slider.nslice),
            padded_pos.x,
            padded_pos.y,
            width + element->slider.padding*2,
            height + element->slider.padding*2
        );
    }

    float value = *(element->slider.value);
    int32_t pin_x = (int32_t)(ceil(((float)element->slider.pin_width)*0.5) + ((float)element->slider.width - element->slider.pin_width)*value);

    // full slider part
    if (element->slider.full_nslice != NULL) {
        draw_nine_slice(
            *(element->slider.full_nslice),
            padded_pos.x + element->slider.padding,
            padded_pos.y + element->slider.padding,
            pin_x,
            element->slider.height
        );
    }
    // empty slider part
    if (element->slider.empty_nslice != NULL) {
        draw_nine_slice(
            *(element->slider.empty_nslice),
            padded_pos.x + element->slider.padding + pin_x,
            padded_pos.y + element->slider.padding,
            element->slider.width - pin_x,
            element->slider.height
        );
    }
    // pin
    if (element->slider.pin_nslice != NULL) {
        draw_nine_slice(
            *(element->slider.pin_nslice),
            padded_pos.x + element->slider.padding + pin_x - element->slider.pin_width*0.5,
            padded_pos.y + element->slider.padding + (element->slider.height - element->slider.pin_height)*0.5,
            element->slider.pin_width,
            element->slider.pin_height
        );
    }
}
void draw_ui_list_button_element(int32_t id, int32_t element_i, int8_t hovered) {
    ivec2_t padded_pos = get_ui_list_element_padded_pos(id, element_i);
    int32_t width = get_ui_list_element_width(id, element_i);
    int32_t height = get_ui_list_element_height(id, element_i);

    if (ui_lists[id].elements[element_i].button.nslice != NULL) {
        draw_nine_slice(
            *(ui_lists[id].elements[element_i].button.nslice),
            padded_pos.x,
            padded_pos.y,
            width,
            height
        );
    }

    // hovering indication
    if (hovered == 1 && ui_lists[id].elements[element_i].button.callback != NULL) {
        if (ui_lists[id].elements[element_i].button.hover_nslice != NULL) {
            draw_nine_slice(
                *(ui_lists[id].elements[element_i].button.hover_nslice),
                padded_pos.x,
                padded_pos.y,
                width,
                height
            );
        }
    }

    draw_string(
        *(ui_lists[id].elements[element_i].button.font),
        ui_lists[id].elements[element_i].button.string,
        (vec3_t){
            .x = padded_pos.x + ui_lists[id].elements[element_i].button.padding,
            .y = padded_pos.y + ui_lists[id].elements[element_i].button.padding,
            .z = 0
        },
        quat_from_axis_angles_yzx(-0, -0, -0),
        (ui_lists[id].elements[element_i].button.font->letter_height),
        1, 1, 1
    );
}
void draw_ui_list_element(int32_t id, int32_t element_i, int8_t hovered) {
    switch (ui_lists[id].elements[element_i].type) {
        case ELEMENT_TYPE_BUTTON: {
            draw_ui_list_button_element(id, element_i, hovered);
            break;
        }
        case ELEMENT_TYPE_SLIDER: {
            draw_ui_list_slider_element(id, element_i, hovered);
            break;
        }
    }
}
void draw_ui_list(int32_t id) {
    if (ui_lists[id].hidden) return;

    ivec2_t padded_pos = get_ui_list_pos_padded(id);

    uint32_t width = get_ui_list_width(id);
    uint32_t height = get_ui_list_height(id);

    ivec2_t in_element = get_ui_list_hovered_element();

    draw_nine_slice(
        *(ui_lists[id].nslice),
        padded_pos.x,
        padded_pos.y,
        width +ui_lists[id].padding*2,
        height+ui_lists[id].padding*2
    );
    
    // elements
    for (int32_t ei = 0; ei < ui_lists[id].elements_amount; ei++) {
        int32_t hovered = 0;
        if (in_element.x == id && in_element.y == ei) hovered = 1;
        draw_ui_list_element(id, ei, hovered);
    }
}
void draw_all_ui_lists() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 0) continue;
        draw_ui_list(i);
    }
    draw_ui_list_hovered_element_info_string();
}