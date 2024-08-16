#include "game.h"

uint8_t init() {
    bind_mesh(man_mesh);
    bind_mesh(cube_mesh);

    // <player>
        player = (player_t){
            .cube = (cube_t){
                .x = 0,
                .y = 80,
                .z = 0,

                .rx = 0,
                .ry = 0,
                .rz = 0,

                .w = 10,
                .h = 40,
                .d = 6
            },
            .can_jump_buffer = 0,
            .vy = 0,

            .current_anim_frame = 0,
            .last_anim_frame = 0,
            .anim_transition_frame = 0,
            .current_anim = man_anim_t_pose,
            .last_anim = man_anim_t_pose
        };
    // </player>

    // <player_camera>
        player_camera = create_camera(
            0, 0, 0,
            0, 0, 0,
            320, 240, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );
    // </player_camera>

    // <cubes>
        cubes[0] = (cube_t){
            .x = 0, .y = -10, .z = 0,
            .rx = 0, .ry = 0, .rz = 0,
            .w = 500, .h = 20, .d = 500
        };
        cubes[1] = (cube_t){
            .x = 40, .y = 20, .z = 40,
            .rx = M_PI*0.7, .ry = M_PI*1.8, .rz = M_PI*0.1,
            .w = 80, .h = 60, .d = 20
        };
        cubes[2] = (cube_t){
            .x = -50, .y = 40, .z = 20,
            .rx = 0, .ry = M_PI*0.2, .rz = M_PI*0.05,
            .w = 50, .h = 10, .d = 40
        };
        
        for (uint64_t i = 0; i < CUBES_AMOUNT; i++) {
            cube_update_aabb(&(cubes[i]));
        }
    // </cubes>

    // <sun shadow map>
        sun_shadow_map_camera = create_camera(
            0, 0, 0,
            M_PI*1.6, M_PI*0.3, 0,
            1080, 1080, 1600,
            -32000, 32000,
            0, 45,
            0, 0, 3240, 3240
        );
    // </sun shadow map>

    outport_fbo = create_fbo(_OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1, GL_RGB, 4);
    
    global_shader = create_shader_from_files(
        "./src/game/shaders/global.vert",
        "./src/game/shaders/global.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        // "u_position\0u_scale\0u_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
        "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
    );

    global_texture = load_texture("./src/game/textures/global_texture.png");

    sun_shadow_map_fbo = create_fbo(3240, 3240, 0, 0, 2);

    sun_shadow_map_shader = create_shader_from_files(
        "./src/game/shaders/sun_shadow_map.vert",
        "./src/game/shaders/sun_shadow_map.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        // "u_position\0u_scale\0u_rotation", 3
        "u_position\0u_scale\0u_quat_rotation", 3
    );


    return 0;
}