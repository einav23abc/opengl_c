#include "game.h"

void game_render() {
    // <sun shadow map>
        use_fbo(sun_shadow_map_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        camera_use(sun_shadow_map_camera);
        shader_use(sun_shadow_map_shader);
        game_render_world();
    // </sun shadow map>
    
    // <player camera>
        camera_use(player_camera);
        use_fbo((fbo_t*)outport_fbo);

        // if (player_camera->is_prespective == 0) {
        //     sdm_set_color(0, 1, 0, 1);
        //     sdm_draw_ball(player_camera->x, player_camera->y, player_camera->z, 5);
        // }
        
        shader_use(global_shader);
        // u_camera_position
        glUniform3f(global_shader->uniform_locations[3], player_camera->x, player_camera->y, player_camera->z);
        // u_sun_vector
        glUniform3f(global_shader->uniform_locations[4], sun_vector_x, sun_vector_y, sun_vector_z);
        // u_sun_shadow_map_wvp_mat
        glUniformMatrix4fv(global_shader->uniform_locations[5], 1, 0, sun_shadow_map_camera->wvp_mat.mat);
        // u_sun_shadow_map_texture
        bind_fbo_depth_stencil_texture(sun_shadow_map_fbo, global_shader->uniform_locations[6], 1);

        game_render_world();
        
        // <AABB>
        if (keys[SDL_SCANCODE_K]) {
            uint64_t i;
            uint8_t j;
        
            glDepthFunc(GL_LEQUAL);
            
            // cubes
            for (i = 0; i < CUBES_AMOUNT; i++) {
                cube_debug_draw_vertices(&(cubes[i]));
            }
            
            // player
            cube_debug_draw_vertices(&(player.cube));

            glDepthFunc(GL_LESS);
        }
        // </AABB>
    // </player camera>

    shader_use((shader_t*)default_shader);

    return;
}

void game_render_world() {
    // <player>
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], player.cube.x, player.cube.y, player.cube.z);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 1, 1, 1);
        // u_rotation
        glUniform3f(shaders_list[current_shader]->uniform_locations[2], 0, -player.cube.ry, 0);
        draw_with_mesh(player_mesh);
    // </player>

    // <cubes>
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (uint64_t i = 0; i < CUBES_AMOUNT; i++){
            cube_draw(&(cubes[i]));
        }
    // </cubes>

    // <man mesh>
        glDisable(GL_CULL_FACE);
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], 0, 0, -60);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 20, 20, 20);
        // u_rotation
        glUniform3f(shaders_list[current_shader]->uniform_locations[2], 0, 0, 0);
        draw_with_mesh(man_mesh);
        glEnable(GL_CULL_FACE);
    // </man mesh>
}