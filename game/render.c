#include "game.h"

void render() {
    // clear outport fbo
    use_fbo(outport_fbo);
    glClearColor(_OUTPORT_BACKGROUND_COLOR_R_, _OUTPORT_BACKGROUND_COLOR_G_, _OUTPORT_BACKGROUND_COLOR_B_, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (page) {
        case PAGE_IN_GAME:
            render_game();
            break;
        
        case PAGE_MAIN_MENU:
            render_main_menu();
            break;
        
        case PAGE_HOW_TO_PLAY:
            render_how_to_play();
            break;
    }

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


void render_game() {
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

    render_game_effects();

    // <ui>
        render_game_ui();
    // <ui>
}

void draw_tile(int32_t player_i, int32_t tile_x, int32_t tile_z, int32_t tile_type, int32_t fixed_shader) {
    int32_t i = player_i;
    int32_t x = tile_x;
    int32_t z = tile_z;

    quat_t quat_rotation;

    switch (tile_type) {
        case TILE_TYPE_HOUSE:
            // u_texture
            bind_texture(house_tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            draw_mesh(house_tile_mesh);
            break;

        case TILE_TYPE_BARRACKS:
            // u_texture
            bind_texture(barracks_tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            draw_mesh(barracks_tile_mesh);
            break;

        case TILE_TYPE_FIELD:
            // u_texture
            bind_texture(field_tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            draw_mesh(field_tile_mesh);
            draw_mesh(field_wheat_tile_mesh);
            break;

        case TILE_TYPE_MINE:
            // u_texture
            bind_texture(mine_tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            draw_mesh(mine_tile_mesh);
            break;

        case TILE_TYPE_FOREST:
            // u_texture
            bind_texture(forest_tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            draw_mesh(forest_tile_mesh);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                0.4*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                0.2*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            draw_mesh(forest_tree_tile_mesh);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                -0.35*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                0.25*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            draw_mesh(forest_tree_tile_mesh);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                0.3*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                -0.4*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            draw_mesh(forest_tree_tile_mesh);
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                -0.3*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                game_struct.players[i].y_current_translation,
                -0.35*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
            );
            draw_mesh(forest_tree_tile_mesh);
            break;
    }
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

    // <scale body>
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);

        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], -1*_TILE_SIZE_, -1*_TILE_SIZE_, 0);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 2*_TILE_SIZE_, _SCALE_AXIS_POINT_Y_ + 2*_TILE_SIZE_, 1*_TILE_SIZE_);
        draw_mesh(cube_mesh);

        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], 0, -1*_TILE_SIZE_, 0.5*_TILE_SIZE_);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
        draw_mesh(scale_base_mesh);
        
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], 0, _SCALE_AXIS_POINT_Y_ + _TILE_SIZE_, 0.5*_TILE_SIZE_);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
        draw_mesh(scale_head_mesh);
    // <scale body>

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
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, _TILE_SIZE_, _TILE_SIZE_);
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(-0, -M_PI*(1-i), -0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        draw_mesh(hinge_mesh);
    }

    // win bases
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], (i*2-1)*(_TILE_SIZE_*(_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_+1+_PLAYER_GRID_WIDTH_*0.5)), _TILE_SIZE_*(-1+0.25), 0);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_*(_PLAYER_GRID_WIDTH_+0.5), _TILE_SIZE_*0.5, _TILE_SIZE_*(_PLAYER_GRID_DEPTH_+0.5));
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        draw_mesh(centered_cube_mesh);
    }

    // <players tiles bases>
    bind_texture(tile_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
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

    // <player tiles>
    // u_texture
    bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
    for (int8_t i = 0; i < 2; i ++) {
        for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
            for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                int32_t tile_type = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].type;
                draw_tile(i, x, z, tile_type, 0);
            }
        }
    }
    // </player tiles>
}

void render_game_effects() {
    glDisable(GL_CULL_FACE);

    // <hovered and selected tiles>
        use_shader(tile_effect_shader);
        // <hovered tile>
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
                glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)154)/255, ((float)64)/255, ((float)126)/255);
                // u_speed
                glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0005);
                // u_freq
                glUniform1f(shaders_list[current_shader]->uniform_locations[5], 5);
                // u_time
                glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
                // u_width
                glUniform1f(shaders_list[current_shader]->uniform_locations[7], -1);
                // u_length
                glUniform1f(shaders_list[current_shader]->uniform_locations[8], -1);
                draw_mesh(tile_effect_mesh);
            }
        // </hovered tile>

        // <selected tile>
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
                glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)216)/255, ((float)128)/255, ((float)56)/255);
                // u_speed
                glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0005);
                // u_freq
                glUniform1f(shaders_list[current_shader]->uniform_locations[5], 5);
                // u_time
                glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
                draw_mesh(tile_effect_mesh);
            }
        // </selected tile>
    // </hovered and selected tiles>

    // <win bases>
        use_shader(tile_effect_shader);
        for (int8_t i = 0; i < 2; i ++) {
            // u_position
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[0],
                -0.1 -_TILE_SIZE_*(_PLAYER_GRID_WIDTH_+0.5)*0.5 + (i*2-1)*(_TILE_SIZE_*(_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_+1+_PLAYER_GRID_WIDTH_*0.5)),
                -_TILE_SIZE_,
                -0.1 -_TILE_SIZE_*(_PLAYER_GRID_DEPTH_+0.5)*0.5
            );
            // u_scale
            glUniform3f(
                shaders_list[current_shader]->uniform_locations[1],
                0.2 + _TILE_SIZE_*(_PLAYER_GRID_WIDTH_+0.5),
                _TILE_SIZE_*1.5,
                0.2 + _TILE_SIZE_*(_PLAYER_GRID_DEPTH_+0.5)
            );
            // u_quat_rotation
            quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            // u_color
            if (game_struct.player_turn == i) {
                glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)216)/255, ((float)128)/255, ((float)56)/255);
            }else {
                glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)154)/255, ((float)64)/255, ((float)126)/255);
            }
            // u_speed
            glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0001);
            // u_freq
            glUniform1f(shaders_list[current_shader]->uniform_locations[5], 10);
            // u_time
            glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
            // u_width
            glUniform1f(shaders_list[current_shader]->uniform_locations[7], -1);
            // u_length
            glUniform1f(shaders_list[current_shader]->uniform_locations[8], -1);
            draw_mesh(tile_effect_mesh);
        }
    // </win bases>

    // <preview building>
        ivec2_t hovered_button = get_ui_list_hovered_button();
        if (hovered_button.x != -1) {
            if (ui_lists[hovered_button.x].button_callbacks[hovered_button.y] == &ui_list_build_specific_button_callback) {
                use_shader(build_preview_shader);
                // u_color
                glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)108)/255, ((float)140)/255, ((float)80)/255);
                // u_speed
                glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.003);
                // u_freq
                glUniform1f(shaders_list[current_shader]->uniform_locations[5], 3);
                // u_time
                glUniform1f(shaders_list[current_shader]->uniform_locations[6], (float)time);
                draw_tile(0, selected_tile.x, selected_tile.y, hovered_button.y, 1);
            }
        }
    // </preview building>

    // <built effect>
        use_shader(tile_effect_shader);
        for (int8_t i = 0; i < 2; i ++) {
            for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
                for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                    int32_t effect_time_to_live = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].built_effect_time_to_live;
                    
                    if (effect_time_to_live <= 0) continue;

                    // u_position
                    glUniform3f(
                        shaders_list[current_shader]->uniform_locations[0],
                        x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        game_struct.players[i].y_current_translation,
                        z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                    );
                    // u_scale
                    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, 0.75*_TILE_SIZE_, _TILE_SIZE_);
                    // u_quat_rotation
                    quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                    // u_color
                    glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)108)/255, ((float)140)/255, ((float)80)/255);
                    // u_speed
                    glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0012);
                    // u_freq
                    glUniform1f(shaders_list[current_shader]->uniform_locations[5], 8);
                    // u_time
                    glUniform1f(shaders_list[current_shader]->uniform_locations[6], _TILE_BUILT_EFFECT_TIME_-effect_time_to_live);
                    // u_width
                    glUniform1f(shaders_list[current_shader]->uniform_locations[7], 0.6);
                    // u_length
                    glUniform1f(shaders_list[current_shader]->uniform_locations[8], _TILE_BUILT_EFFECT_TIME_);
                    draw_mesh(tile_effect_mesh);
                }
            }
        }
        use_shader(ui_shader);
    // </built effect>
    
    // <destroyed effect>
        use_shader(tile_effect_shader);
        for (int8_t i = 0; i < 2; i ++) {
            for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
                for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                    int32_t effect_time_to_live = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].destroyed_effect_time_to_live;
                    
                    if (effect_time_to_live <= 0) continue;

                    // u_position
                    glUniform3f(
                        shaders_list[current_shader]->uniform_locations[0],
                        x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        game_struct.players[i].y_current_translation,
                        z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_
                    );
                    // u_scale
                    glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_SIZE_, 0.75*_TILE_SIZE_, _TILE_SIZE_);
                    // u_quat_rotation
                    quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                    // u_color
                    glUniform3f(shaders_list[current_shader]->uniform_locations[3], ((float)78)/255, ((float)40)/255, ((float)46)/255);
                    // u_speed
                    glUniform1f(shaders_list[current_shader]->uniform_locations[4], 0.0012);
                    // u_freq
                    glUniform1f(shaders_list[current_shader]->uniform_locations[5], 8);
                    // u_time
                    glUniform1f(shaders_list[current_shader]->uniform_locations[6], _TILE_DESTROYED_EFFECT_TIME_-effect_time_to_live);
                    // u_width
                    glUniform1f(shaders_list[current_shader]->uniform_locations[7], 0.6);
                    // u_length
                    glUniform1f(shaders_list[current_shader]->uniform_locations[8], _TILE_DESTROYED_EFFECT_TIME_);
                    draw_mesh(tile_effect_mesh);
                }
            }
        }
        use_shader(ui_shader);
    // </destroyed effect>

    glEnable(GL_CULL_FACE);
}

void render_game_ui() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    billboard_t billboard;
    
    // <cooldown billboards>
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
                        .x = -8,
                        .y = 8,
                        .box_width = 16,
                        .box_height = 16
                    };
                    // u_rads
                    glUniform1f(
                        shaders_list[current_shader]->uniform_locations[2],
                        M_PI*2*(tile->curent_cooldown_timer / ((float)tile_type_cooldown))
                    );
                    draw_billboard_shaded(billboard);
                }
            }
        }
        use_shader(ui_shader);
    // </cooldown billboards>
    
    // <attacked billboards>
        use_shader(attacked_billboards_shader);
        // u_texture
        bind_texture(attack_effect_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (int8_t i = 0; i < 2; i ++) {
            for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
                for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                    int32_t effect_time_to_live = game_struct.players[i].tiles[z*_PLAYER_GRID_DEPTH_ + x].attacked_effect_time_to_live;
                    
                    if (effect_time_to_live <= 0) continue;

                    billboard = (billboard_t){
                        .box_pos_from_world_pos = 1,
                        .box_world_pos_x = 0.5*_TILE_SIZE_ + x*_TILE_SIZE_ + game_struct.players[i].x_current_translation,
                        .box_world_pos_y = 0.25*_TILE_SIZE_ + game_struct.players[i].y_current_translation,
                        .box_world_pos_z = 0.5*_TILE_SIZE_ + z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
                        .x = -32,
                        .y = 32,
                        .box_width = 64,
                        .box_height = 64
                    };
                    // u_frames_amount
                    glUniform1i(
                        shaders_list[current_shader]->uniform_locations[2],
                        6
                    );
                    // u_current_frame
                    glUniform1i(
                        shaders_list[current_shader]->uniform_locations[3],
                        // animate at 20fps
                        (int32_t)((_TILE_ATTACKED_EFFECT_TIME_ - effect_time_to_live)/(1000.0/20))
                    );
                    draw_billboard_shaded(billboard);
                }
            }
        }
        use_shader(ui_shader);
    // </attacked billboards>

    // <player 0 resources>
        char resources_string[66] = "";

        resources_t player_resources = game_struct.players[0].resources;
        resources_t resources_delta = (resources_t){
            .population = 0,
            .soldiers = 0,
            .stone = 0,
            .wheat = 0,
            .wood = 0
        };
        ivec2_t hovered_button = get_ui_list_hovered_button();
        if (hovered_button.x != -1) {
            if (hovered_button.y >= 0 && hovered_button.y < ui_lists[hovered_button.x].buttons_amount) {
                if (ui_lists[hovered_button.x].button_callbacks[hovered_button.y] == &ui_list_build_specific_button_callback) {
                    resources_delta = tile_type_properties[hovered_button.y].cost;
                }
            }
        }
        player_resources.population -=  resources_delta.population;
        player_resources.soldiers -=    resources_delta.soldiers;
        player_resources.stone -=       resources_delta.stone;
        player_resources.wheat -=       resources_delta.wheat;
        player_resources.wood -=        resources_delta.wood;
        
        int32_t c = 0;

        if (resources_delta.population > 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xa1;
            resources_string[c+2] = 0x3d;
            resources_string[c+3] = 0x3b;
            c += 4;
        }
        resources_string[c] = ' ';
        if (player_resources.population >= 10) {
            resources_string[c]  = '0' + player_resources.population/10;
        }else if (player_resources.population < 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0x1c;
            resources_string[c+2] = 0x16;
            resources_string[c+3] = 0x18;
            c += 4;
            resources_string[c]  = '-';
        }
        resources_string[c+1]  = '0' + abs(player_resources.population % 10);
        c += 2;
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xf9;
            resources_string[c+2] = 0xf5;
            resources_string[c+3] = 0xef;
            c += 4;
        resources_string[c  ] = 0x11;
        resources_string[c+1] = 0x12;
        c += 2;
        
        if (resources_delta.wheat > 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xa1;
            resources_string[c+2] = 0x3d;
            resources_string[c+3] = 0x3b;
            c += 4;
        }
        resources_string[c] = ' ';
        if (player_resources.wheat >= 10) {
            resources_string[c]  = '0' + player_resources.wheat/10;
        }else if (player_resources.wheat < 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0x1c;
            resources_string[c+2] = 0x16;
            resources_string[c+3] = 0x18;
            c += 4;
            resources_string[c]  = '-';
        }
        resources_string[c+1]  = '0' + abs(player_resources.wheat % 10);
        c += 2;
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xf9;
            resources_string[c+2] = 0xf5;
            resources_string[c+3] = 0xef;
            c += 4;
        resources_string[c  ] = 0x13;
        resources_string[c+1] = 0x14;
        c += 2;
        
        if (resources_delta.wood > 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xa1;
            resources_string[c+2] = 0x3d;
            resources_string[c+3] = 0x3b;
            c += 4;
        }
        resources_string[c] = ' ';
        if (player_resources.wood >= 10) {
            resources_string[c]  = '0' + player_resources.wood/10;
        }else if (player_resources.wood < 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0x1c;
            resources_string[c+2] = 0x16;
            resources_string[c+3] = 0x18;
            c += 4;
            resources_string[c]  = '-';
        }
        resources_string[c+1]  = '0' + abs(player_resources.wood % 10);
        c += 2;
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xf9;
            resources_string[c+2] = 0xf5;
            resources_string[c+3] = 0xef;
            c += 4;
        resources_string[c  ] = 0x15;
        resources_string[c+1] = 0x16;
        c += 2;
        
        if (resources_delta.stone > 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xa1;
            resources_string[c+2] = 0x3d;
            resources_string[c+3] = 0x3b;
            c += 4;
        }
        resources_string[c] = ' ';
        if (player_resources.stone >= 10) {
            resources_string[c]  = '0' + player_resources.stone/10;
        }else if (player_resources.stone < 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0x1c;
            resources_string[c+2] = 0x16;
            resources_string[c+3] = 0x18;
            c += 4;
            resources_string[c]  = '-';
        }
        resources_string[c+1]  = '0' + abs(player_resources.stone % 10);
        c += 2;
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xf9;
            resources_string[c+2] = 0xf5;
            resources_string[c+3] = 0xef;
            c += 4;
        resources_string[c  ] = 0x17;
        resources_string[c+1] = 0x18;
        c += 2;
        
        if (resources_delta.soldiers > 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xa1;
            resources_string[c+2] = 0x3d;
            resources_string[c+3] = 0x3b;
            c += 4;
        }
        resources_string[c] = ' ';
        if (player_resources.soldiers >= 10) {
            resources_string[c]  = '0' + player_resources.soldiers/10;
        }else if (player_resources.soldiers < 0) {
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0x1c;
            resources_string[c+2] = 0x16;
            resources_string[c+3] = 0x18;
            c += 4;
            resources_string[c]  = '-';
        }
        resources_string[c+1]  = '0' + abs(player_resources.soldiers % 10);
        c += 2;
            resources_string[c  ] = 0x1f;
            resources_string[c+1] = 0xf9;
            resources_string[c+2] = 0xf5;
            resources_string[c+3] = 0xef;
            c += 4;
        resources_string[c  ] = 0x19;
        resources_string[c+1] = 0x1a;
        c += 2;

        resources_string[c] = '\0';

        uvec2_t str_box_size = get_str_boxed_size(resources_string, big_letters_font.letter_height);
        draw_str_boxed(resources_string, big_letters_font, nine_slice1, _OUTPORT_WIDTH_-str_box_size.x-6, 6, 6, big_letters_font.letter_height);
    // </player 0 resources>

    draw_all_ui_lists();
    draw_all_alerts();
    
    glEnable(GL_DEPTH_TEST);
}



void render_main_menu() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    // scrolled background
    // u_texture
    bind_texture(menu_background_scroll_texture, shaders_list[current_shader]->u_texture_loc, 0);
    int32_t d = ((int32_t)(time*0.01) % 128);
    for (int32_t x = -1; x < (_OUTPORT_WIDTH_/128.0)+1; x++) {
        for (int32_t y = -1; y < (_OUTPORT_HEIGHT_/128.0)+1; y++) {
            
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                (float)(x*128 + d),
                (float)(y*128 - d)
            );
            // u_scale
            glUniform2f(shaders_list[current_shader]->uniform_locations[1], 128, 128);
            draw_mesh(rect_plane_mesh);
        }
    }


    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}

void render_how_to_play() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    // scrolled background
    // u_texture
    bind_texture(menu_background_scroll_texture, shaders_list[current_shader]->u_texture_loc, 0);
    int32_t d = ((int32_t)(time*0.01) % 128);
    for (int32_t x = -1; x < (_OUTPORT_WIDTH_/128.0)+1; x++) {
        for (int32_t y = -1; y < (_OUTPORT_HEIGHT_/128.0)+1; y++) {
            
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                (float)(x*128 + d),
                (float)(y*128 - d)
            );
            // u_scale
            glUniform2f(shaders_list[current_shader]->uniform_locations[1], 128, 128);
            draw_mesh(rect_plane_mesh);
        }
    }

    char str[] =
        ""      "The game is played using only the mouse.\n"
        "\n"
        ""      "    Build structures in your side of\n"
        ""      "        the grid to gain wheight.\n"
        ""      "The first player to be heavier by 5 wins.\n"
        "\n"
        ""      " Think strategicly about what you build!\n"
        ""      " Using strategies simmilar to those used\n"
        ""      "  in 'Settlers of catan' is beneficial.\n"
        "\n"
        ""      "     Using the 'Soldiers' resource,\n"
        ""      "  you can attack the enemy's buildings."
    ;
    uvec2_t size = get_str_boxed_size(str, big_letters_font.letter_height);
    draw_str_boxed(
        str,
        big_letters_font,
        nine_slice1,
        _OUTPORT_WIDTH_*0.5  - size.x*0.5 - 12,
        _OUTPORT_HEIGHT_*0.5 - size.y*0.5 - 12,
        12,
        big_letters_font.letter_height
    );


    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}