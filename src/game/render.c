#include "game.h"

void render() {
    if (in_game) {
        render_game();
        return;
    }
    
    render_load_game_screen();
}

void render_load_game_screen() {
    use_camera(default_camera);

    glDisable(GL_DEPTH_TEST);

    sdm_set_color(1, 1, 1, 1);
    sdm_draw_rect(0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_);
    
    sdm_set_color(0.2, 0.2, 0.2, 1);
    sdm_draw_rect(_OUTPORT_WIDTH_*0.2, _OUTPORT_HEIGHT_*0.4, _OUTPORT_WIDTH_*0.6, _OUTPORT_HEIGHT_*0.2);
    
    sdm_set_color(0.8, 0.0, 0.0, 1);
    sdm_draw_rect(_OUTPORT_WIDTH_*0.2, _OUTPORT_HEIGHT_*0.4, _OUTPORT_WIDTH_*0.6 * (((float)load_game_progress)/LOAD_GAME_PROGRESS_MAX), _OUTPORT_HEIGHT_*0.2);
    
    glEnable(GL_DEPTH_TEST);
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
    
    // <player camera>
        use_camera(player_camera);
        use_fbo(outport_fbo);

        // if (player_camera->is_prespective == 0) {
        //     sdm_set_color(0, 1, 0, 1);
        //     sdm_draw_ball(player_camera->x, player_camera->y, player_camera->z, 5);
        // }
        
        use_shader(global_shader);
        // u_camera_position
        glUniform3f(global_shader->uniform_locations[3], player_camera->x, player_camera->y, player_camera->z);
        // u_sun_vector
        glUniform3f(global_shader->uniform_locations[4], sun_vector_x, sun_vector_y, sun_vector_z);
        // u_sun_shadow_map_wvp_mat
        glUniformMatrix4fv(global_shader->uniform_locations[5], 1, 0, sun_shadow_map_camera->wvp_mat.mat);
        // u_sun_shadow_map_texture
        bind_fbo_depth_stencil_texture(sun_shadow_map_fbo, global_shader->uniform_locations[6], 1);

        render_game_world();
        
        // <AABB>
        if (keys[SDL_SCANCODE_K]) {
            uint64_t i;
            uint8_t j;

            glDepthFunc(GL_LEQUAL);
            
            // cubes
            for (i = 0; i < CUBES_AMOUNT; i++) {
                cube_debug_draw_vertices(&(cubes[i]));
            }
            
            // players
            for (int32_t i = 0; i <_PLAYERS_MAX_AMOUNT_; i++) {
                if (players[i].connected == 0) continue;
                
                cube_debug_draw_vertices(&(players[i].cube));
            }

            glDepthFunc(GL_LESS);
        }
        // </AABB>
    // </player camera>
    

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

    // <players>
        for (int32_t i = 0; i < _PLAYERS_MAX_AMOUNT_; i++) {
            if (players[i].connected == 0) continue;

            bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
            // u_position
            glUniform3f(shaders_list[current_shader]->uniform_locations[0], players[i].cube.x, players[i].cube.y, players[i].cube.z);
            // u_scale
            glUniform3f(shaders_list[current_shader]->uniform_locations[1], 5, 5, 5);
            // u_quat_rotation
            quat_rotation = quat_from_axis_angles_yzx(0, -players[i].cube.ry, 0);
            glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
            pose_mesh_set_from_animation(man_mesh, players[i].last_anim, fmod(players[i].last_anim_frame*0.015, 0.75));
            pose_mesh_mix_from_animation(man_mesh, players[i].current_anim, fmod(players[i].current_anim_frame*0.015, 0.75), (players[i].anim_transition_frame*0.015)/0.25);
            
            // pose_mesh_set_from_animation(man_mesh, man_anim_t_pose, 0);
            draw_mesh_posed(man_mesh);
            // draw_mesh(man_mesh);
        }
    // </players>

    // <cubes>
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (uint64_t i = 0; i < CUBES_AMOUNT; i++){
            cube_draw(&(cubes[i]));
        }
    // </cubes>
}

void cube_draw(cube_t* cube) {
    // u_position
    glUniform3f(shaders_list[current_shader]->uniform_locations[0], cube->x, cube->y, cube->z);
    // u_scale
    glUniform3f(shaders_list[current_shader]->uniform_locations[1], cube->w, cube->h, cube->d);
    // u_quat_rotation
    quat_t quat_rotation = quat_from_axis_angles_yzx(-cube->rx, -cube->ry, -cube->rz);
    glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
    draw_mesh(cube_mesh);
}

void cube_debug_draw_vertices(cube_t* cube) {
    sdm_set_color(1, 0, 0, 1);
    for (uint8_t j = 0; j < 8; j++) {
        sdm_draw_ball(cube->vertices[j].x, cube->vertices[j].y, cube->vertices[j].z, 5);
    }
}