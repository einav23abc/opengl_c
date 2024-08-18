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
shader_t* cooldown_billboards_shader;
shader_t* tile_effect_shader;

vec3_t camera_pos;
camera_t* camera;
camera_t* ui_camera;

mesh_t* rect_plane_mesh;
mesh_t* cube_mesh;
mesh_t* centered_cube_mesh;
mesh_t* tile_effect_mesh;

tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

game_t game_struct;
ivec3_t selected_tile;
ivec2_t hovered_tiles[2];
int8_t in_cooldowns_translation;
int8_t in_tiles_translation;
int8_t player1_ai_played;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;
shader_t* sun_shadow_map_shader;


float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g) {
    int32_t string_len = min(strlen(str), _MAX_TEXT_ROW_LENGTH);

    shader_t* last_shader = shaders_list[current_shader];
    use_shader(font_shader);

    // u_texture
    bind_texture(font.font_texture, shaders_list[current_shader]->u_texture_loc, 0);
    // u_position
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[0],
        pos.x,
        pos.y,
        pos.z
    );
    // u_scale
    float size_x = string_len*font.letter_width*(height/font.letter_height);
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[1],
        size_x,
        height,
        0
    );
    // u_quat_rotation
    glUniform4f(
        shaders_list[current_shader]->uniform_locations[2],
        rot.x,
        rot.y,
        rot.z,
        rot.w
    );
    // u_text_row_length
    glUniform1i(
        shaders_list[current_shader]->uniform_locations[3],
        string_len
    );
    // u_text_row
    int32_t text_row[string_len];
    for (int32_t i = 0; i < string_len; i++) {
        text_row[i] = str[i];
    }
    glUniform1iv(
        shaders_list[current_shader]->uniform_locations[4],
        string_len,
        text_row
    );
    // u_font_data
    glUniform2i(
        shaders_list[current_shader]->uniform_locations[5],
        font.letters_in_row,
        font.letters_in_col
    );
    // u_color
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[6],
        color_r,
        color_g,
        color_b
    );
    draw_mesh(rect_plane_mesh);

    use_shader(last_shader);

    return size_x;
}
uvec2_t get_str_boxed_size(char* str, float row_height) {
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
    uvec2_t str_size = get_str_boxed_size(str, row_height);
    
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


int32_t has_enough_resources(int32_t player_id, int32_t tile_type_id) {
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);

    if (
        game_struct.players[player_id].resources.wood       < tile_type->cost.wood       ||
        game_struct.players[player_id].resources.stone      < tile_type->cost.stone      ||
        game_struct.players[player_id].resources.wheat      < tile_type->cost.wheat      ||
        game_struct.players[player_id].resources.population < tile_type->cost.population ||
        game_struct.players[player_id].resources.soldiers   < tile_type->cost.soldiers
    ) {
        return 0;
    }
    return 1;
}

void switch_turn_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.player_turn == 1) {
        add_error_alert_at_cursor("It is not your turn");
        return;
    }
    switch_turn();
}
void switch_turn() {
    game_struct.player_turn = !game_struct.player_turn;
    selected_tile.x = -1;
    selected_tile.y = -1;
    close_unperm_ui_lists();

    player1_ai_played = 0;

    // update cooldown timers
    for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
        for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
            tile_t* tile = &(game_struct.players[game_struct.player_turn].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
            
            if (tile->type == TILE_TYPE_EMPTY) continue;

            tile->cooldown_timer -= 1;

            if (tile->cooldown_timer <= 0) {
                tile->cooldown_timer = tile_type_properties[tile->type].give_cooldown;
            }
        }
    }
}
void player_1_turn() {
    player_1_ai_turn();
}
void player_1_ai_turn() {
    // dont resolve turn while transitions (animations) are playing
    if (in_cooldowns_translation == 1) return;
    if (in_tiles_translation == 1) return;

    if (player1_ai_played == 1) {
        switch_turn();
        return;
    }

    // count amounts
    int32_t tile_types_amounts[_TILE_TYPES_AMOUNT_];
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) tile_types_amounts[i] = 0;
    for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
        for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
            tile_t* tile = &(game_struct.players[game_struct.player_turn].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
            if (tile->type == TILE_TYPE_EMPTY) continue;
            tile_types_amounts[tile->type] += 1;
        }
    }

    // sort amounts
    int32_t sorted_tile_type_id[_TILE_TYPES_AMOUNT_];
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) sorted_tile_type_id[i] = i;
    // bubble sort
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_-1; i++) {
        for (uint32_t j = 0; j < _TILE_TYPES_AMOUNT_-1-i; j++) {
            if (
                (tile_types_amounts[sorted_tile_type_id[j]] > tile_types_amounts[sorted_tile_type_id[j+1]]) ||
                // put some randomness into equal amount tiles
                (
                    tile_types_amounts[sorted_tile_type_id[j]] == tile_types_amounts[sorted_tile_type_id[j+1]] &&
                    rand()%2 == 1
                )
            ) {
                int32_t tmp = sorted_tile_type_id[j];
                sorted_tile_type_id[j  ] = sorted_tile_type_id[j+1];
                sorted_tile_type_id[j+1] = tmp;
            }
        }
    }

    // build whatever i can
    while(1) {
        uint8_t built = 0;
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_-1; i++) {
            if (has_enough_resources(1, sorted_tile_type_id[i]) == 0) continue;

            // find space to build
            int32_t new_tile_grid_index = -1;
            for (uint32_t j = 0; j < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; j++) {
                if (game_struct.players[1].tiles[j].type == TILE_TYPE_EMPTY) {
                    new_tile_grid_index = j;
                    break;
                }
            }
            // handle no space
            if (new_tile_grid_index == -1) break;

            int32_t tile_type_id = sorted_tile_type_id[i];
            tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);

            game_struct.players[1].wheight += 1;
            game_struct.players[0].wheight -= 1;
            game_struct.players[1].resources.wood       -= tile_type->cost.wood;
            game_struct.players[1].resources.stone      -= tile_type->cost.stone;
            game_struct.players[1].resources.wheat      -= tile_type->cost.wheat;
            game_struct.players[1].resources.population -= tile_type->cost.population;
            game_struct.players[1].resources.soldiers   -= tile_type->cost.soldiers;
            game_struct.players[1].tiles[new_tile_grid_index].cooldown_timer        = tile_type->give_cooldown;
            game_struct.players[1].tiles[new_tile_grid_index].curent_cooldown_timer = tile_type->give_cooldown;
            game_struct.players[1].tiles[new_tile_grid_index].type = tile_type_id;
        }
        if (built == 0) break;
    }

    // end turn after transitions will end
    player1_ai_played = 1;
}