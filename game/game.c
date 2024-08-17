#include "game.h"


// variable declarations

int32_t page;

fbo_t* outport_fbo;

texture_t* floor_texture;
texture_t* global_texture;

font_t letters_font;

shader_t* global_shader;
shader_t* ui_shader;
shader_t* font_shader;

vec3_t camera_pos;
camera_t* camera;
camera_t* ui_camera;

mesh_t* rect_plane_mesh;
mesh_t* cube_mesh;
mesh_t* centered_cube_mesh;

tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

game_t game_struct;
ivec2_t selected_tile;
ivec2_t hovered_tiles[2];

ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];
alert_t alerts[_MAX_ALERTS_AMOUNT_];

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;
shader_t* sun_shadow_map_shader;



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
        if (ui_lists[i].safe == 0) ui_lists[i].active = 0;
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


uvec2_t get_str_size(char* str, float row_height) {
    uint32_t w = 0;
    uint32_t h = row_height;
    uint32_t x = 0;

    int32_t str_len = strlen(str);
    char ch;

    for (uint32_t c = 0; c < str_len; c++) {
        ch = str[c];
        if (ch == '\n') {
            x = 0;
            h += row_height;
            continue;
        }

        x += 1;
        w = max(w, x);
    }

    return (uvec2_t){
        .x = w * letters_font.letter_width*(row_height/letters_font.letter_height),
        .y = h
    };
}
void draw_str_boxed(char* str, uint32_t left_x, uint32_t bottom_y, uint32_t padding, uint32_t row_height) {
    uvec2_t str_size = get_str_size(str, row_height);
    
    uint32_t top_y = bottom_y + str_size.y;

    uint32_t x = 0;
    uint32_t y = -row_height;

    sdm_set_color(0,0.25,0,1);
    sdm_draw_rect(
        left_x - padding,
        top_y - str_size.y - padding,
        2*padding + str_size.x,
        2*padding + str_size.y
    );
    
    char one_char_str[2] = "X\0";
    int32_t info_str_len = strlen(str);

    for (uint32_t c = 0; c < info_str_len; c++) {
        one_char_str[0] = str[c];
        if (one_char_str[0] == '\n') {
            x = 0;
            y -= row_height;
            continue;
        }

        if (x >= str_size.x) {
            x = 0;
            y -= row_height;
        }

        x += draw_string(
            letters_font,
            one_char_str,
            (vec3_t){
                .x = left_x + x,
                .y = top_y + y,
                .z = 0
            },
            quat_from_axis_angles_yzx(-0, -0, -0),
            row_height,
            1, 1, 1
        );
    }
}

uvec2_t get_ui_button_info_size(char* info_str) {
    return get_str_size(info_str, _UI_LIST_BUTTON_INFO_ROW_HEIGHT_);
}


int32_t new_alert_assign_id() {
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live <= 0) return i;
    }
    // get shortest time to live alert
    int32_t min_time_to_live = alerts[0].time_to_live;
    int32_t min_time_to_live_i = 0;
    for (int32_t i = 1; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live < min_time_to_live) {
            min_time_to_live = alerts[i].time_to_live;
            min_time_to_live_i = i;
        }
    }
    return min_time_to_live_i;
}
uvec2_t get_alert_box_size(int32_t i) {
    return get_str_size(alerts[i].string, _ALERT_ROW_HEIGHT_);
}
uvec2_t get_alert_box_pos(int32_t i) {
    float x = 0;
    float y = 0;
    if (alerts[i].box_pos_from_world_pos == 1) {
        vec2_t screen_cord = outport_space_position_from_world_space(vec3(alerts[i].box_world_pos_x, alerts[i].box_world_pos_y, alerts[i].box_world_pos_z));
        x = screen_cord.x;
        y = screen_cord.y;
    }

    x += alerts[i].x;
    y += alerts[i].y;

    if (&(alerts[i].easing_function) != NULL) {
        float t = 1-(((float)alerts[i].time_to_live) / (alerts[i].initial_time_to_live));
        y += alerts[i].easing_function(t) * alerts[i].y_full_transform;
    }

    if (x < _ALERT_PADDING_) x = _ALERT_PADDING_;
    if (y < _ALERT_PADDING_) y = _ALERT_PADDING_;

    return (uvec2_t){
        .x = x,
        .y = y
    };
}
uvec2_t get_alert_box_pos_padded(int32_t i) {
    uvec2_t box_pos = get_alert_box_pos(i);
    box_pos.x -= _ALERT_PADDING_;
    box_pos.y -= _ALERT_PADDING_;
    return box_pos;
}
void add_alert_at_cursor(char* string) {
    vec2_t mouse_outport_space_position = get_mouse_outport_space_position();

    int32_t new_alert_id = new_alert_assign_id();

    uvec2_t string_size = get_str_size(string, _ALERT_ROW_HEIGHT_);
    
    alerts[new_alert_id] = (alert_t){
        .time_to_live = 3000,

        .initial_time_to_live = 3000,
        .y_full_transform = string_size.y*3,
        .easing_function = &ease_out_sine,

        .box_pos_from_world_pos = 0,
        .x = mouse_outport_space_position.x - string_size.x*0.5,
        .y = mouse_outport_space_position.y - string_size.y*0.5,

        .string = string
    };
}
void close_all_alerts() {
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;
}


vec2_t outport_space_position_from_world_space(vec3_t pos) {
    // for `camera`
    vec2_t osp_pos = vec2_from_vec4(vec4_mul_by_mat4(vec4(pos.x,pos.y,pos.z,1.0), camera->world_view_projection_matrix));
    return vec2_mul(
        vec2_add(vec2_mul_by_scalar(osp_pos, 0.5),vec2(0.5,0.5)),
        vec2(
            _OUTPORT_WIDTH_,
            _OUTPORT_HEIGHT_
        )
    );
}
vec2_t get_mouse_outport_space_position() {
    uint32_t outport_fbo_pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t outport_fbo_w = _OUTPORT_WIDTH_*outport_fbo_pixel_scale;
    uint32_t outport_fbo_h = _OUTPORT_HEIGHT_*outport_fbo_pixel_scale;

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*_OUTPORT_WIDTH_ /outport_fbo_w + _OUTPORT_WIDTH_*0.5,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*_OUTPORT_HEIGHT_/outport_fbo_h + _OUTPORT_HEIGHT_*0.5
    };
}
vec2_t get_mouse_camera_space_position() {
    uint32_t outport_fbo_pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t outport_fbo_w = _OUTPORT_WIDTH_*outport_fbo_pixel_scale;
    uint32_t outport_fbo_h = _OUTPORT_HEIGHT_*outport_fbo_pixel_scale;

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*camera->width /outport_fbo_w,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*camera->height/outport_fbo_h
    };
}
vec3_t get_mouse_world_space_position_at_y(float at_y) {
    // assuming using `camera`-ortho and `outport_fbo`
    // very unoptimised, quickly hacked together

    vec3_t mouse_camera_space_position = vec3_from_vec2(get_mouse_camera_space_position());

    quat_t camera_rot_quat = quat_from_axis_angles_yzx(-camera->rx, -camera->ry, -camera->rz);
    vec3_t mouse_as_camera_plane_translation = rotate_vector(mouse_camera_space_position, camera_rot_quat);

    vec3_t pos = {
        .x = camera_pos.x + mouse_as_camera_plane_translation.x,
        .y = camera_pos.y + mouse_as_camera_plane_translation.y,
        .z = camera_pos.z + mouse_as_camera_plane_translation.z
    };

    vec3_t camera_in_translation = rotate_vector((vec3_t){0,0,1}, camera_rot_quat);

    // find intersection with top of player[0].tiles
    float t = (at_y-pos.y)/camera_in_translation.y;
    pos = vec3_add(pos, vec3_mul_by_scalar(camera_in_translation, t));
    
    return pos;
}
ivec2_t get_hovered_tile_position(uint8_t player_i) {
    vec3_t world_space_position = get_mouse_world_space_position_at_y(game_struct.players[player_i].y_current_translation);

    return (ivec2_t){
        .x = (int32_t)floor((world_space_position.x - game_struct.players[player_i].x_current_translation)/_TILE_SIZE_),
        .y = (int32_t)floor((world_space_position.z - _PLAYER_CONSTANT_Z_TRANSLATION_)                    /_TILE_SIZE_)
    };
}


void switch_turn() {
    game_struct.player_turn = !game_struct.player_turn;
    selected_tile.x = -1;
    selected_tile.y = -1;
    close_all_ui_lists();
}
void player_1_turn() {
    player_1_ai_turn();
}
void player_1_ai_turn() {

}