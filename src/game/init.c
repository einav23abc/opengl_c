#include "game.h"

uint8_t init() {
    frames = 0;

    default_camera = camera_create(
        OUTPORT_WIDTH*0.5, OUTPORT_HEIGHT*0.5, 0,
        0, 0, 0,
        OUTPORT_WIDTH, OUTPORT_HEIGHT, 1600,
        -32000, 32000,
        0, 60,
        0, 0, 320, 240
    );


    in_game = 0;
    load_game_progress = 0;
    thread_create(
        &load_game_thread,
        NULL,
        (void*)&load_game,
        NULL
    );

    
    global_shader = shader_create_from_files(
        "./src/game/shaders/global.vert",
        "./src/game/shaders/global.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        // "u_position\0u_scale\0u_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
        "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
    );
    load_game_progress += 1;

    global_texture = load_texture("./src/game/textures/global_texture.png");
    load_game_progress += 1;

    sun_shadow_map_fbo = create_fbo(
        3240, 3240,
        0, 0, NULL, 0,
        2, NULL, 0
    );
    load_game_progress += 1;

    sun_shadow_map_shader = shader_create_from_files(
        "./src/game/shaders/sun_shadow_map.vert",
        "./src/game/shaders/sun_shadow_map.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        // "u_position\0u_scale\0u_rotation", 3
        "u_position\0u_scale\0u_quat_rotation", 3
    );
    load_game_progress += 1;
    
    return 0;
}


void load_game() {
    // create new opengl context
    // each thread needs a unique context to call opengl functions
    // the new context doesnt share 'container objects' with the main thread -
    // - due to this, shaders and fbo arent loaded and all meshes are loaded unbinded and must be binded by the main thread
    // the context is deleted before exiting the thread

    // <man mesh>
        // man_mesh = mesh_from_wavefront_obj("./src/game/models/man.obj");
        // man_mesh = mesh_from_collada_dae_ext(
        //     "./src/game/models/man_rigged.dae",
        //     (quat_vec_vec_t){
        //         .rot = quat_from_axis_angles_yzx(M_PI*1.5, 0, 0),
        //         .scale = (vec3_t){
        //             .x = 1,
        //             .y = 1,
        //             .z = 1
        //         },
        //         .pos = (vec3_t){
        //             .x = 0,
        //             .y = 0,
        //             .z = 0
        //         }
        //     }
        // );
        man_mesh = mesh_from_collada_dae("./src/game/models/man_rigged.dae", 1);
    // </man mesh>
    load_game_progress += 1;

    // <animation of man mesh>
        anim = animation_from_collada_dae_ext(
            "./src/game/models/man_rigged.dae",
            man_mesh->joints,
            man_mesh->joints_amount,
            (quat_vec_vec_t){
                .rot = quat_from_axis_angles_yzx(M_PI*1.5, 0, 0),
                .scale = (vec3_t){
                    .x = 1,
                    .y = 1,
                    .z = 1
                },
                .pos = (vec3_t){
                    .x = 0,
                    .y = -4.3,
                    .z = -0.3
                }
            }
        );
        // anim = animation_from_collada_dae(
        //     "./src/game/models/man_rigged.dae",
        //     man_mesh->joints,
        //     man_mesh->joints_amount
        // );
        // for (uint32_t i = 0; i < anim->joints_amount; i++) {
        //     printf("joint %d \"%s\" :\n\t[\n", i, man_mesh->joints[i].name);
        //     for (uint32_t j = 0; j < anim->joints_key_frames[i].key_frames_amount; j++) {
        //         printf(
        //             "\t\t%f :\n",
        //             anim->joints_key_frames[i].key_frames[j].time_stamp
        //         );
        //         print_mat4(anim->joints_key_frames[i].key_frames[j].joint_local_transform);
        //         print_quat_vec_vec(anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv);
        //     }
        //     printf("\t]\n");
        // }
    // </animation of man mesh>
    load_game_progress += 1;
    

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
            .vy = 0
        };

        player_camera = camera_create(
            0, 0, 0,
            0, 0, 0,
            320, 240, 1600,
            -32000, 32000,
            0, 60,
            0, 0, 320, 240
        );
    // </player>
    load_game_progress += 1;
    
    // <cube_mesh>
        if(1){
        float vertices_position_arr[] = {
            // front
            -0.5,-0.5,-0.5,
            -0.5, 0.5,-0.5,
             0.5, 0.5,-0.5,
             0.5,-0.5,-0.5,
            // back
            -0.5,-0.5, 0.5,
            -0.5, 0.5, 0.5,
             0.5, 0.5, 0.5,
             0.5,-0.5, 0.5,
            // bottom
            -0.5,-0.5,-0.5,
            -0.5,-0.5, 0.5,
             0.5,-0.5, 0.5,
             0.5,-0.5,-0.5,
            // top
            -0.5, 0.5,-0.5,
            -0.5, 0.5, 0.5,
             0.5, 0.5, 0.5,
             0.5, 0.5,-0.5,
            // left
            -0.5,-0.5,-0.5,
            -0.5, 0.5,-0.5,
            -0.5, 0.5, 0.5,
            -0.5,-0.5, 0.5,
            // right
             0.5,-0.5,-0.5,
             0.5, 0.5,-0.5,
             0.5, 0.5, 0.5,
             0.5,-0.5, 0.5,
        };
        float vertices_texcoord_arr[] = {
            // front
            0,1,
            0,0,
            1,0,
            1,1,
            // back
            1,1,
            1,0,
            0,0,
            0,1,
            // bottom
            0,1,
            0,0,
            1,0,
            1,1,
            // top
            0,1,
            0,0,
            1,0,
            1,1,
            // left
            1,1,
            1,0,
            0,0,
            0,1,
            // right
            0,1,
            0,0,
            1,0,
            1,1,
        };
        float vertices_normal_arr[] = {
            // front
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            // back
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            // bottom
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            // top
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            // left
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            // right
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
        };
        vbo_data_t vbo_datas_arr[3] = {
            {
                .data_arr_size = sizeof(vertices_position_arr),
                .data_arr = (void*)vertices_position_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_texcoord_arr),
                .data_arr = (void*)vertices_texcoord_arr,
                .size = 2,
                .type = GL_FLOAT,
                .stride = 2*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_normal_arr),
                .data_arr = (void*)vertices_normal_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            }
        };
        
        uint32_t indices_array[] = {
            // front
            1, 0, 2,
            2, 0, 3,
            // back
            4, 5, 6,
            4, 6, 7,
            // bottom
            8, 9,10,
            8,10,11,
            // top
            13,12,14,
            14,12,15,
            // left
            16,17,18,
            16,18,19,
            // right
            21,20,22,
            22,20,23,
        };
        cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 1);
        };
    // </cube_mesh>
    load_game_progress += 1;
    
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
    load_game_progress += 1;

    // <sun shadow map>
        sun_shadow_map_camera = camera_create(
            0, 0, 0,
            M_PI*1.6, M_PI*0.3, 0,
            1080, 1080, 1600,
            -32000, 32000,
            0, 45,
            0, 0, 3240, 3240
        );
    // </sun shadow map>
    load_game_progress += 1;


    exit_thread(0);
}