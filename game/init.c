#include "game.h"

uint8_t init() {
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
        cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 0);
        // bind_mesh(cube_mesh);
        };
    // </cube_mesh>

    // <camera>
        camera_pos = (vec3_t){
            .x = 0,
            .y = 0,
            .z = 0
        };
        camera = create_camera(
            0, 0, 0,
            0, 0, 0,
            320, 240, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );
    // </camera>

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
        "./game/shaders/global.vert",
        "./game/shaders/global.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
    );

    global_texture = load_texture("./game/textures/global_texture.png");
    // save_surface_to_c_file("./game/textures/global_texture.png", "global_texture_surface", "./game/textures/global_texture_surface.c");
    
    sun_shadow_map_fbo = create_fbo(3240, 3240, 0, 0, 2);
    
    sun_shadow_map_shader = create_shader_from_files(
        "./game/shaders/sun_shadow_map.vert",
        "./game/shaders/sun_shadow_map.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation", 3
    );


    return 0;
}