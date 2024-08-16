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
    
    // <camera>
        use_camera(camera);
        use_fbo(outport_fbo);

        sdm_set_color(1, 0, 1, 1);
        sdm_draw_ball(camera_pos.x, camera_pos.y, camera_pos.z, 5);
        
        vec3_t mouse_world_space_position = get_mouse_world_space_position_at_y(_TILE_HEIGHT_*0.5);
        sdm_set_color(0, 0, 1, 1);
        sdm_draw_ball(mouse_world_space_position.x, mouse_world_space_position.y, mouse_world_space_position.z, 5);
        
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
        
    // </camera>
    

    // draw outport frame buffer to screen
    use_default_fbo();

    float pixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
    // uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    glViewport((window_drawable_width-w)*0.5,(window_drawable_height-h)*0.5,w,h);
    
    simple_draw_module_draw_fbo_color_texture(outport_fbo);
}

void render_game_world() {
    quat_t quat_rotation;

    // <player[0].tiles>
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
            for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
                // u_position
                glUniform3f(shaders_list[current_shader]->uniform_locations[0], x*_TILE_WIDTH_, 0*_TILE_HEIGHT_, z*_TILE_DEPTH_);
                // u_scale
                glUniform3f(shaders_list[current_shader]->uniform_locations[1], _TILE_WIDTH_, _TILE_HEIGHT_, _TILE_DEPTH_);
                // u_quat_rotation
                quat_t quat_rotation = quat_from_axis_angles_yzx(-0, -0, -0);
                glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
                draw_mesh(cube_mesh);
            }
        }
    // </player[0].tiles>
}
