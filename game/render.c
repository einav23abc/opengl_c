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

    // <hovered and selected tiles>
        use_shader(tile_effect_shader);
        glDisable(GL_CULL_FACE);

        // hovered tile
        for (int8_t i = 0; i < 2; i ++) {
            if (
                hovered_tiles[i].x < 0 ||
                hovered_tiles[i].x >= _PLAYER_GRID_WIDTH_ ||
                hovered_tiles[i].y < 0 ||
                hovered_tiles[i].y >= _PLAYER_GRID_DEPTH_
            ) continue;

            // dont draw on selected tile
            if (
                i == selected_tile.z &&
                hovered_tiles[i].x == selected_tile.x &&
                hovered_tiles[i].y == selected_tile.y
            ) continue;

            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                hovered_tiles[i].x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                hovered_tiles[i].y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_*0.5, _TILE_SIZE_);
            // u_quat_rotation
            quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            // u_color
            glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)108)/255, ((float)140)/255, ((float)80)/255);
            // u_speed
            glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0005);
            // u_freq
            glUniform1f(shaders_list[current_shader]->uniform_locations[5], 5);
            // u_time
            glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
            draw_mesh(tile_effect_mesh);
        }

        // selected tile
        if (
            selected_tile.x >= 0 &&
            selected_tile.x < _PLAYER_GRID_WIDTH_ &&
            selected_tile.y >= 0 &&
            selected_tile.y < _PLAYER_GRID_DEPTH_
        ) {
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                selected_tile.x*_TILE_SIZE_ + game_struct.players[selected_tile.z].x_current_translation,
                game_struct.players[selected_tile.z].y_current_translation,
                selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_*0.5, _TILE_SIZE_);
            // u_quat_rotation
            quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            // u_color
            glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)227)/255, ((float)210)/255, ((float)69)/255);
            // u_speed
            glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0005);
            // u_freq
            glUniform1f(shaders_list[current_shader]->uniform_locations[5], 5);
            // u_time
            glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
            draw_mesh(tile_effect_mesh);
        }
        glEnable(GL_CULL_FACE);
    // </hovered and selected tiles>

    // <ui>
        use_fbo(outport_fbo);
        use_camera(ui_camera);
        use_shader(ui_shader);

        render_game_ui();
    // <ui>
    

    // draw outport frame buffer to screen
    use_default_fbo();

    uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
        w = _OUTPORT_WIDTH_*fpixel_scale;
        h = _OUTPORT_HEIGHT_*fpixel_scale;
    }
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
        resources_string[0] = '0' + game_struct.players[0].resources.population;
        resources_string[2] = '0' + game_struct.players[0].resources.wheat;
        resources_string[4] = '0' + game_struct.players[0].resources.wood;
        resources_string[6] = '0' + game_struct.players[0].resources.stone;
        resources_string[8] = '0' + game_struct.players[0].resources.soldiers;
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
                int32_t tile_type = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].type;
                
                switch (tile_type) {
                    case TILE_TYPE_HOUSE:
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
                        break;

                    case TILE_TYPE_BARRACKS:
                        // u_position
                        glUniform3f(
                            shaders_list[current_shader]->uniform_locations[0],
                            (((float)x)+0.125)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            game_struct.players[i].y_current_translation,
                            (((float)z)+0.375)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                        );
                        // u_scale
                        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.75, _TILE_SIZE_*0.5, _TILE_SIZE_*0.25);
                        // u_quat_rotation
                        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                        draw_mesh(cube_mesh);
                        
                        // u_position
                        glUniform3f(
                            shaders_list[current_shader]->uniform_locations[0],
                            (((float)x)+0.375)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            game_struct.players[i].y_current_translation,
                            (((float)z)+0.125)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                        );
                        // u_scale
                        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.25, _TILE_SIZE_*0.5, _TILE_SIZE_*0.75);
                        // u_quat_rotation
                        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                        draw_mesh(cube_mesh);
                        break;

                    case TILE_TYPE_FIELD:
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
                        break;

                    case TILE_TYPE_MINE:
                        // u_position
                        glUniform3f(
                            shaders_list[current_shader]->uniform_locations[0],
                            (((float)x)+0.125)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            game_struct.players[i].y_current_translation,
                            (((float)z)+0.25)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                        );
                        // u_scale
                        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.75, _TILE_SIZE_*0.5, _TILE_SIZE_*0.5);
                        // u_quat_rotation
                        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                        draw_mesh(cube_mesh);
                        break;

                    case TILE_TYPE_FOREST:
                        // u_position
                        glUniform3f(
                            shaders_list[current_shader]->uniform_locations[0],
                            (((float)x)+0.125)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            game_struct.players[i].y_current_translation,
                            (((float)z)+0.125)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                        );
                        // u_scale
                        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.25, _TILE_SIZE_*1, _TILE_SIZE_*0.25);
                        // u_quat_rotation
                        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                        draw_mesh(cube_mesh);
                        
                        // u_position
                        glUniform3f(
                            shaders_list[current_shader]->uniform_locations[0],
                            (((float)x)+0.625)*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                            game_struct.players[i].y_current_translation,
                            (((float)z)+0.625)*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                        );
                        // u_scale
                        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*0.25, _TILE_SIZE_*1, _TILE_SIZE_*0.25);
                        // u_quat_rotation
                        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                        draw_mesh(cube_mesh);
                        break;
                }
            }
        }
    }
    // </player tiles>
}


void render_game_ui() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);


    // <cooldown billboards>
        quat_t quat_rotation;
        billboard_t billboard;
        use_shader(cooldown_billboards_shader);
        for (int8_t i = 0; i < 2; i ++) {
            for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
                for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                    tile_t* tile = &(game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
                    int32_t tile_type_cooldown = tile_type_properties[tile->type].give_cooldown;
                    
                    if (tile->type == TILE_TYPE_EMPTY) continue;
                    if (tile->curent_cooldown_timer <= 0 || tile->curent_cooldown_timer >= tile_type_cooldown) continue;

                    billboard = (billboard_t){
                        .box_pos_from_world_pos = 1,
                        .box_world_pos_x = 0.5*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        .box_world_pos_y = 0.5*_TILE_SIZE_ + game_struct.players[i].y_current_translation,
                        .box_world_pos_z = 0.5*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
                        .x = -10,
                        .y = 10,
                        .box_width = 20,
                        .box_height = 20
                    };
                    glUniform1f(
                        cooldown_billboards_shader->uniform_locations[2],
                        M_PI*2*(tile->curent_cooldown_timer / ((float)tile_type_cooldown))
                    );
                    draw_billboard_shaded(billboard);
                }
            }
        }
        use_shader(ui_shader);
    // </cooldown billboards>


    // draw ui lists
    int32_t ui_list_to_draw_info_string = -1;
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
                // hovering indication
                if (cursor_inside_button == j && ui_lists[i].button_callbacks[j] != NULL) {
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
                ui_list_to_draw_info_string = i;
            }
        }
    }

    // ui list button info string
    if (ui_list_to_draw_info_string != -1) {
        uvec2_t box_pos = get_ui_list_box_pos(ui_list_to_draw_info_string);
        int32_t cursor_inside_button = floor(((float)cursor_inside_box_pos.y)/_UI_LIST_BUTTON_HEIGHT_);

        uvec2_t box_size = get_str_boxed_size(ui_lists[ui_list_to_draw_info_string].button_info_strings[cursor_inside_button], _UI_LIST_BUTTON_HEIGHT_);

        uint32_t left_x = box_pos.x + cursor_inside_box_pos.x + _UI_LIST_PADDING_;
        uint32_t bottom_y = box_pos.y + _UI_LIST_BUTTON_HEIGHT_ + cursor_inside_button*_UI_LIST_BUTTON_INFO_ROW_HEIGHT_ + _UI_LIST_PADDING_;

        if (left_x - _UI_LIST_PADDING_ <= 0) {
            left_x = _UI_LIST_PADDING_;
        }
        if (left_x + box_size.x + _UI_LIST_PADDING_ >= _OUTPORT_WIDTH_) {
            left_x = _OUTPORT_WIDTH_ - _UI_LIST_PADDING_ - box_size.x;
        }
        if (bottom_y - _UI_LIST_PADDING_ <= 0) {
            bottom_y = _UI_LIST_PADDING_;
        }
        if (bottom_y + box_size.y + _UI_LIST_PADDING_ >= _OUTPORT_HEIGHT_) {
            bottom_y = _OUTPORT_HEIGHT_ - _UI_LIST_PADDING_ - box_size.y;
        }

        draw_str_boxed(ui_lists[ui_list_to_draw_info_string].button_info_strings[cursor_inside_button], left_x, bottom_y, _UI_LIST_PADDING_, _UI_LIST_BUTTON_INFO_ROW_HEIGHT_);
    }

    // draw alerts
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live <= 0) continue;
        uvec2_t box_pos = get_alert_box_pos(i);
        uint32_t left_x = box_pos.x;
        uint32_t bottom_y = box_pos.y;
        draw_str_boxed(alerts[i].string, left_x, bottom_y, _ALERT_PADDING_, _ALERT_ROW_HEIGHT_);
    }

    glEnable(GL_DEPTH_TEST);
}