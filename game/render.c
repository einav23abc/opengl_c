#include "game.h"

void render() {
    render_game();
}


void render_game() {
    // clear outport fbo
    use_fbo(outport_fbo);
    glClearColor(_OUTPORT_BACKGROUND_COLOR_R_, _OUTPORT_BACKGROUND_COLOR_G_, _OUTPORT_BACKGROUND_COLOR_B_, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // <sun shadow map>
        use_fbo(sun_shadow_map_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        use_camera(sun_shadow_map_camera);
        use_shader(sun_shadow_map_shader);

        render_game_world();
    // </sun shadow map>
    
    // <game world>
        use_camera(camera);
        use_fbo(outport_fbo);

        sdm_set_color(1, 0, 1, 1);
        sdm_draw_ball(camera_pos.x, camera_pos.y, camera_pos.z, 5);

        use_shader(global_shader);
        // u_camera_position
        glUniform3f(global_shader->uniform_locations[3], camera->x, camera->y, camera->z);
        // u_sun_vector
        glUniform3f(global_shader->uniform_locations[4], sun_vector_x, sun_vector_y, sun_vector_z);
        // u_sun_shadow_map_wvp_mat
        glUniformMatrix4fv(global_shader->uniform_locations[5], 1, 0, sun_shadow_map_camera->wvp_mat.mat);
        // u_sun_shadow_map_texture
        bind_fbo_depth_stencil_texture(sun_shadow_map_fbo, global_shader->uniform_locations[6], 1);

        render_game_world();
    // </game world>

    // <ui>
        use_fbo(outport_fbo);
        use_camera(ui_camera);
        use_shader(ui_shader);

        render_game_ui();
    // <ui>
    

    // draw outport frame buffer to screen
    use_default_fbo();

    // float pixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
    uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    glViewport((window_drawable_width-w)*0.5,(window_drawable_height-h)*0.5,w,h);
    
    simple_draw_module_draw_fbo_color_texture(outport_fbo);
}

void render_game_world() {
    quat_t quat_rotation;

    // giant floor
    // u_texture
    bind_texture(floor_texture, shaders_list[current_shader]->u_texture_loc, 0);
    // u_position
    glUniform3f(shaders_list[current_shader]->uniform_locations[0], -50*_TILE_SIZE_, -_TILE_SIZE_-10, -50*_TILE_SIZE_);
    // u_scale
    glUniform3f(shaders_list[current_shader]->uniform_locations[1], 100*_TILE_SIZE_, 10, 100*_TILE_SIZE_);
    // u_quat_rotation
    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
    draw_mesh(cube_mesh);

    // scale body
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    // u_position
    glUniform3f(shaders_list[current_shader]->uniform_locations[0], -1*_TILE_SIZE_, -1*_TILE_SIZE_, 0);
    // u_scale
    glUniform3f(shaders_list[current_shader]->uniform_locations[1], 2*_TILE_SIZE_, _SCALE_AXIS_POINT_Y_ + 2*_TILE_SIZE_, 1*_TILE_SIZE_);
    // u_quat_rotation
    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
    draw_mesh(cube_mesh);

    // scale axis
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    // u_position
    glUniform3f(shaders_list[current_shader]->uniform_locations[0], 0, _SCALE_AXIS_POINT_Y_, -0.5*_TILE_SIZE_);
    // u_scale
    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _SCALE_AXIS_LENGTH_*2, _TILE_SIZE_, _TILE_SIZE_);
    // u_quat_rotation
    float rot = -asin((game_struct.players[1].hinge_y_position - _SCALE_AXIS_POINT_Y_)/_SCALE_AXIS_LENGTH_);
    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -rot);
    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
    draw_mesh(centered_cube_mesh);

    // scale hinges
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], game_struct.players[i].hinge_x_position, game_struct.players[i].hinge_y_position, 0);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 2*_TILE_SIZE_, 2*_TILE_SIZE_, _TILE_SIZE_);
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        draw_mesh(centered_cube_mesh);
    }

    // win bases
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], (i*2-1)*(_TILE_SIZE_*(_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_+1+_PLAYER_GRID_WIDTH_*0.5)), _TILE_SIZE_*(-1+0.25), 0);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*_PLAYER_GRID_WIDTH_, _TILE_SIZE_*0.5, _TILE_SIZE_*_PLAYER_GRID_DEPTH_);
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        draw_mesh(centered_cube_mesh);
    }

    // <players tiles bases>
    for (int8_t i = 0; i < 2; i ++) {
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
            for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                // u_position
                glUniform3f(
                    shaders_list[current_shader]->uniform_locations[0],
                    x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                    -_TILE_SIZE_ + game_struct.players[i].y_current_translation,
                    z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                );
                // u_scale
                glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
                // u_quat_rotation
                quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                draw_mesh(cube_mesh);
            }
        }
    }
    // </players tiles bases>
    
    // <player 0 resources on tile base>
        glDisable(GL_DEPTH_TEST);
        char resources_string[11] = "X\01X\02X\03X\04X\05";
        resources_string[0] = '0' + game_struct.players[0].population;
        resources_string[2] = '0' + game_struct.players[0].wheat;
        resources_string[4] = '0' + game_struct.players[0].wood;
        resources_string[6] = '0' + game_struct.players[0].stone;
        resources_string[8] = '0' + game_struct.players[0].soldiers;
        draw_string(
            letters_font,
            resources_string,
            (vec3_t){
                .x = game_struct.players[0].x_current_translation,
                .y = -_TILE_SIZE_ + game_struct.players[0].y_current_translation,
                .z = _PLAYER_CONSTANT_Z_TRANSLATION_
            },
            quat_from_axis_angles_yzx(-0, -0, -0),
            _TILE_SIZE_,
            1, 1, 1
        );
        glEnable(GL_DEPTH_TEST);
    // </player 0 resources on tile base>

    // <player tiles>
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
        for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
            for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                uint8_t hovered = (hovered_tiles[i].x == x && hovered_tiles[i].y == z);
                uint8_t selected = (selected_tile.x == x && selected_tile.y == z && i == 0);
                int32_t tile_type = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].type;
                
                if (tile_type != TILE_TYPE_EMPTY) {
                    // u_position
                    glUniform3f(
                        shaders_list[current_shader]->uniform_locations[0],
                        (((float)x)+0.25)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        game_struct.players[i].y_current_translation,
                        (((float)z)+0.25)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                    );
                    // u_scale
                    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.5, _TILE_SIZE_*0.75, _TILE_SIZE_*0.5);
                    // u_quat_rotation
                    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                    draw_mesh(cube_mesh);
                }

                
                if (selected) {
                    // u_position
                    glUniform3f(
                        shaders_list[current_shader]->uniform_locations[0],
                        (((float)x)+0.125)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        game_struct.players[i].y_current_translation,
                        (((float)z)+0.125)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                    );
                    // u_scale
                    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.75, _TILE_SIZE_*0.25, _TILE_SIZE_*0.75);
                    // u_quat_rotation
                    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                    draw_mesh(cube_mesh);
                }

                if (hovered) {
                    // u_position
                    glUniform3f(
                        shaders_list[current_shader]->uniform_locations[0],
                        (((float)x)+0.25)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        game_struct.players[i].y_current_translation,
                        (((float)z)+0.25)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                    );
                    // u_scale
                    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.5, _TILE_SIZE_*1.25, _TILE_SIZE_*0.5);
                    // u_quat_rotation
                    quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                    draw_mesh(cube_mesh);
                }
            }
        }
    }
    // </player tiles>

}


void render_game_ui() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // draw ui lists
    ivec3_t cursor_inside_box_pos = get_ui_list_inside_pos();
    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) {
        if (ui_lists[i].active == 1) {

            uvec2_t box_pos = get_ui_list_box_pos(i);
            uvec2_t box_padded_pos = get_ui_list_box_pos_padded(i);
            int32_t cursor_inside_button = -1;
            if (cursor_inside_box_pos.z == i) {
                cursor_inside_button = floor(((float)cursor_inside_box_pos.y)/_UI_LIST_BUTTON_HEIGHT_);
            }

            uint32_t box_width = get_ui_list_width(i);
            uint32_t box_height = get_ui_list_height(i);

            sdm_set_color(1,0,0,1);
            sdm_draw_rect(box_padded_pos.x, box_padded_pos.y, box_width+_UI_LIST_PADDING_*2, box_height+_UI_LIST_PADDING_*2);
            
            // buttons
            for (int32_t j = 0; j < ui_lists[i].buttons_amount; j++) {
                if (cursor_inside_button == j) {
                    sdm_set_color(0.5,0.5,0,1);
                    sdm_draw_rect(box_pos.x, box_pos.y + j*_UI_LIST_BUTTON_HEIGHT_, box_width, _UI_LIST_BUTTON_HEIGHT_);
                }
                draw_string(
                    letters_font,
                    ui_lists[i].button_strings[j],
                    (vec3_t){
                        .x = box_pos.x,
                        .y = box_pos.y + j*_UI_LIST_BUTTON_HEIGHT_,
                        .z = 0
                    },
                    quat_from_axis_angles_yzx(-0, -0, -0),
                    _UI_LIST_BUTTON_HEIGHT_,
                    1, 1, 1
                );
            }
            
            // button info string
            if (cursor_inside_box_pos.z == i) {
                if (cursor_inside_button < 0 || cursor_inside_button >= ui_lists[i].buttons_amount) continue;

                if (ui_lists[i].button_info_strings[cursor_inside_button][0] == '\0') continue;


                uvec2_t info_size = get_ui_button_info_size(ui_lists[i].button_info_strings[cursor_inside_button]);
                
                uint32_t left_x = box_pos.x + cursor_inside_box_pos.x + _UI_LIST_PADDING_;
                uint32_t top_y = box_padded_pos.y + box_height+_UI_LIST_PADDING_*2 + _UI_LIST_PADDING_ + info_size.y - _UI_LIST_BUTTON_INFO_ROW_HEIGHT_;
                uint32_t x = 0;
                uint32_t y = 0;

                sdm_set_color(0,0.25,0,1);
                sdm_draw_rect(
                    box_pos.x + cursor_inside_box_pos.x,
                    box_padded_pos.y + box_height+_UI_LIST_PADDING_*2,
                    2*_UI_LIST_PADDING_ + info_size.x,
                    2*_UI_LIST_PADDING_ + info_size.y
                );
                
                char one_char_str[2] = "X\0";
                int32_t info_str_len = strlen(ui_lists[i].button_info_strings[cursor_inside_button]);

                for (uint32_t c = 0; c < info_str_len; c++) {
                    one_char_str[0] = ui_lists[i].button_info_strings[cursor_inside_button][c];
                    if (one_char_str[0] == '\n') {
                        x = 0;
                        y -= _UI_LIST_BUTTON_INFO_ROW_HEIGHT_;
                        continue;
                    }

                    if (x >= info_size.x) {
                        x = 0;
                        y -= _UI_LIST_BUTTON_INFO_ROW_HEIGHT_;
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
                        _UI_LIST_BUTTON_INFO_ROW_HEIGHT_,
                        1, 1, 1
                    );
                }
            }
        }
    }

    // draw alerts
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live <= 0) continue;

        uvec2_t box_pos = get_alert_box_pos(i);
        uvec2_t box_padded_pos = get_alert_box_pos_padded(i);
        uvec2_t box_size = get_alert_box_size(i);

        uint32_t left_x = box_pos.x;
        uint32_t top_y = box_pos.y;
        uint32_t x = 0;
        uint32_t y = 0;

        sdm_set_color(0,0.25,0,1);
        sdm_draw_rect(
            box_padded_pos.x,
            box_padded_pos.y,
            2*_ALERT_PADDING_ + box_size.x,
            2*_ALERT_PADDING_ + box_size.y
        );
        
        char one_char_str[2] = "X\0";
        int32_t str_len = strlen(alerts[i].string);

        for (uint32_t c = 0; c < str_len; c++) {
            one_char_str[0] = alerts[i].string[c];
            if (one_char_str[0] == '\n') {
                x = 0;
                y -= _ALERT_ROW_HEIGHT_;
                continue;
            }

            if (x >= box_size.x) {
                x = 0;
                y -= _ALERT_ROW_HEIGHT_;
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
                _ALERT_ROW_HEIGHT_,
                1, 1, 1
            );
        }
    }

    glEnable(GL_DEPTH_TEST);
}

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