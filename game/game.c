#include "game.h"


// variable declarations

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

game_t game_struct;
ivec2_t selected_tile;
ivec2_t hovered_tiles[2];

ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];

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
    ui_lists[i].active = 0;
    if (ui_lists[i].child_ui_list >= 0) close_ui_list(ui_lists[i].child_ui_list);
}
void make_ui_list_safe(int32_t i) {
    ui_lists[i].safe = 1;
    if (ui_lists[i].parent_ui_list >= 0) make_ui_list_safe(ui_lists[i].parent_ui_list);
}
void close_unsafe_ui_lists() {
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].safe == 0) ui_lists[i].active = 0;
    }
}
uvec2_t get_ui_list_box_pos(int32_t i) {
    float x;
    float y;
    if (ui_lists[i].box_pos_from_world_pos == 1) {
        vec2_t screen_cord = outport_space_position_from_world_space(vec3(ui_lists[i].box_world_pos_x, ui_lists[i].box_world_pos_y, ui_lists[i].box_world_pos_z));
        x = screen_cord.x;
        y = screen_cord.y;
    }else {
        x = ui_lists[i].x;
        y = ui_lists[i].y;
    }
    if (ui_lists[i].height_down) {
        y -= ui_lists[i].box_h;
    }

    return (uvec2_t){
        .x = x,
        .y = y
    };
}
ivec3_t get_ui_list_inside_pos() {
    vec2_t outport_space_position = get_mouse_outport_space_position();
    
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        uvec2_t box_pos = get_ui_list_box_pos(i);
        if (
            ui_lists[i].active == 1 &&
            box_pos.x <= outport_space_position.x &&
            box_pos.y <= outport_space_position.y &&
            box_pos.x + ui_lists[i].box_w >= outport_space_position.x &&
            box_pos.y + ui_lists[i].box_h >= outport_space_position.y
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