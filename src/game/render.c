#include "game.h"

void render() {
    if (in_game) {
        render_game();
        return;
    }
    
    render_load_game_screen();
}

void render_load_game_screen() {
    camera_use(default_camera);

    glDisable(GL_DEPTH_TEST);

    sdm_set_color(1, 1, 1, 1);
    sdm_draw_rect(0, 0, OUTPORT_WIDTH, OUTPORT_HEIGHT);
    
    sdm_set_color(0.2, 0.2, 0.2, 1);
    sdm_draw_rect(OUTPORT_WIDTH*0.2, OUTPORT_HEIGHT*0.4, OUTPORT_WIDTH*0.6, OUTPORT_HEIGHT*0.2);
    
    sdm_set_color(0.8, 0.0, 0.0, 1);
    sdm_draw_rect(OUTPORT_WIDTH*0.2, OUTPORT_HEIGHT*0.4, OUTPORT_WIDTH*0.6 * (((float)load_game_progress)/LOAD_GAME_PROGRESS_MAX), OUTPORT_HEIGHT*0.2);

    glEnable(GL_DEPTH_TEST);
}

void render_game() {
    // <sun shadow map>
        use_fbo(sun_shadow_map_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        camera_use(sun_shadow_map_camera);
        shader_use(sun_shadow_map_shader);
        render_game_world();
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
            
            // player
            cube_debug_draw_vertices(&(player.cube));

            glDepthFunc(GL_LESS);
        }
        // </AABB>
    // </player camera>

    shader_use((shader_t*)default_shader);
}

void render_game_world() {
    quat_t quat_rotation;

    // <player>
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], player.cube.x, player.cube.y, player.cube.z);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 5, 5, 5);
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(0, -player.cube.ry, 0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        pose_mesh_set_from_animation(man_mesh, player.last_anim, fmod(player.last_anim_frame*0.015, 0.75));
        pose_mesh_mix_from_animation(man_mesh, player.current_anim, fmod(player.current_anim_frame*0.015, 0.75), (player.anim_transition_frame*0.015)/0.25);
        draw_mesh_posed(man_mesh);
    // </player>

    // <cubes>
        // u_texture
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        for (uint64_t i = 0; i < CUBES_AMOUNT; i++){
            cube_draw(&(cubes[i]));
        }
    // </cubes>

    // <man mesh>
        bind_texture(global_texture, shaders_list[current_shader]->u_texture_loc, 0);
        // u_position
        glUniform3f(shaders_list[current_shader]->uniform_locations[0], 0, 80, -60);
        // u_scale
        glUniform3f(shaders_list[current_shader]->uniform_locations[1], 20, 20, 20);
        // u_quat_rotation
        quat_rotation = quat_from_axis_angles_yzx(0, 0, 0);
        glUniform4f(shaders_list[current_shader]->uniform_locations[2], quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);
        pose_mesh_set_from_animation(man_mesh, man_anim_run, fmod(frames*0.015, 0.75));
        draw_mesh_posed(man_mesh);
    // </man mesh>
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