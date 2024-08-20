#include "game.h"

uint8_t init() {
    // <meshes>
        // <cube mesh & centered cube mesh>
            // if(1){
            // float vertices_position_arr[] = {
            //     // front
            //     0,0,0,
            //     0,1,0,
            //     1,1,0,
            //     1,0,0,
            //     // back
            //     0,0,1,
            //     0,1,1,
            //     1,1,1,
            //     1,0,1,
            //     // bottom
            //     0,0,0,
            //     0,0,1,
            //     1,0,1,
            //     1,0,0,
            //     // top
            //     0,1,0,
            //     0,1,1,
            //     1,1,1,
            //     1,1,0,
            //     // left
            //     0,0,0,
            //     0,1,0,
            //     0,1,1,
            //     0,0,1,
            //     // right
            //     1,0,0,
            //     1,1,0,
            //     1,1,1,
            //     1,0,1,
            // };
            // float vertices_texcoord_arr[] = {
            //     // front
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // back
            //     1,1,
            //     1,0,
            //     0,0,
            //     0,1,
            //     // bottom
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // top
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // left
            //     1,1,
            //     1,0,
            //     0,0,
            //     0,1,
            //     // right
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            // };
            // float vertices_normal_arr[] = {
            //     // front
            //     0,0,-1,
            //     0,0,-1,
            //     0,0,-1,
            //     0,0,-1,
            //     // back
            //     0,0,1,
            //     0,0,1,
            //     0,0,1,
            //     0,0,1,
            //     // bottom
            //     0,-1,0,
            //     0,-1,0,
            //     0,-1,0,
            //     0,-1,0,
            //     // top
            //     0,1,0,
            //     0,1,0,
            //     0,1,0,
            //     0,1,0,
            //     // left
            //     -1,0,0,
            //     -1,0,0,
            //     -1,0,0,
            //     -1,0,0,
            //     // right
            //     1,0,0,
            //     1,0,0,
            //     1,0,0,
            //     1,0,0,
            // };
            // vbo_data_t vbo_datas_arr[3] = {
            //     {
            //         .data_arr_size = sizeof(vertices_position_arr),
            //         .data_arr = (void*)vertices_position_arr,
            //         .size = 3,
            //         .type = GL_FLOAT,
            //         .stride = 3*sizeof(float),
            //         .divisor = 0
            //     },
            //     {
            //         .data_arr_size = sizeof(vertices_texcoord_arr),
            //         .data_arr = (void*)vertices_texcoord_arr,
            //         .size = 2,
            //         .type = GL_FLOAT,
            //         .stride = 2*sizeof(float),
            //         .divisor = 0
            //     },
            //     {
            //         .data_arr_size = sizeof(vertices_normal_arr),
            //         .data_arr = (void*)vertices_normal_arr,
            //         .size = 3,
            //         .type = GL_FLOAT,
            //         .stride = 3*sizeof(float),
            //         .divisor = 0
            //     }
            // };
            
            // uint32_t indices_array[] = {
            //     // front
            //     1, 0, 2,
            //     2, 0, 3,
            //     // back
            //     4, 5, 6,
            //     4, 6, 7,
            //     // bottom
            //     8, 9,10,
            //     8,10,11,
            //     // top
            //     13,12,14,
            //     14,12,15,
            //     // left
            //     16,17,18,
            //     16,18,19,
            //     // right
            //     21,20,22,
            //     22,20,23,
            // };

            // cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 1);
            // save_mesh_to_c_file(cube_mesh, "cube_mesh", "./game/models/cube_mesh.c");
            bind_mesh(cube_mesh);
            
            // for (uint32_t i = 0; i <sizeof(vertices_position_arr)/sizeof(float); i++) vertices_position_arr[i] -= 0.5;
            // centered_cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 1);
            // save_mesh_to_c_file(centered_cube_mesh, "centered_cube_mesh", "./game/models/centered_cube_mesh.c");
            bind_mesh(centered_cube_mesh);
            // };
        // </cube mesh & centered cube mesh>

        // <hinge_mesh>
            // hinge_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/hinge.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0,
            //             .y = 0,
            //             .z = 0
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.5,
            //             .y = 0.5,
            //             .z = 0.5
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(hinge_mesh, "hinge_mesh", "./game/models/hinge_mesh.c");
            bind_mesh(hinge_mesh);
        // </hinge_mesh>

        // <tile_effect_mesh>
            // if(1){
            // float vertices_position_arr[] = {
            //     // front
            //     0,0,0,
            //     0,1,0,
            //     1,1,0,
            //     1,0,0,
            //     // back
            //     0,0,1,
            //     0,1,1,
            //     1,1,1,
            //     1,0,1,
            //     // bottom
            //     0,0,0,
            //     0,0,1,
            //     1,0,1,
            //     1,0,0,
            //     // top
            //     0,1,0,
            //     0,1,1,
            //     1,1,1,
            //     1,1,0,
            //     // left
            //     0,0,0,
            //     0,1,0,
            //     0,1,1,
            //     0,0,1,
            //     // right
            //     1,0,0,
            //     1,1,0,
            //     1,1,1,
            //     1,0,1,
            // };
            // float vertices_texcoord_arr[] = {
            //     // front
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // back
            //     1,1,
            //     1,0,
            //     0,0,
            //     0,1,
            //     // bottom
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // top
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            //     // left
            //     1,1,
            //     1,0,
            //     0,0,
            //     0,1,
            //     // right
            //     0,1,
            //     0,0,
            //     1,0,
            //     1,1,
            // };
            // float vertices_normal_arr[] = {
            //     // front
            //     0,0,-1,
            //     0,0,-1,
            //     0,0,-1,
            //     0,0,-1,
            //     // back
            //     0,0,1,
            //     0,0,1,
            //     0,0,1,
            //     0,0,1,
            //     // bottom
            //     0,-1,0,
            //     0,-1,0,
            //     0,-1,0,
            //     0,-1,0,
            //     // top
            //     0,1,0,
            //     0,1,0,
            //     0,1,0,
            //     0,1,0,
            //     // left
            //     -1,0,0,
            //     -1,0,0,
            //     -1,0,0,
            //     -1,0,0,
            //     // right
            //     1,0,0,
            //     1,0,0,
            //     1,0,0,
            //     1,0,0,
            // };
            // vbo_data_t vbo_datas_arr[3] = {
            //     {
            //         .data_arr_size = sizeof(vertices_position_arr),
            //         .data_arr = (void*)vertices_position_arr,
            //         .size = 3,
            //         .type = GL_FLOAT,
            //         .stride = 3*sizeof(float),
            //         .divisor = 0
            //     },
            //     {
            //         .data_arr_size = sizeof(vertices_texcoord_arr),
            //         .data_arr = (void*)vertices_texcoord_arr,
            //         .size = 2,
            //         .type = GL_FLOAT,
            //         .stride = 2*sizeof(float),
            //         .divisor = 0
            //     },
            //     {
            //         .data_arr_size = sizeof(vertices_normal_arr),
            //         .data_arr = (void*)vertices_normal_arr,
            //         .size = 3,
            //         .type = GL_FLOAT,
            //         .stride = 3*sizeof(float),
            //         .divisor = 0
            //     }
            // };
            
            // uint32_t indices_array[] = {
            //     // front
            //     1, 0, 2,
            //     2, 0, 3,
            //     // back
            //     4, 5, 6,
            //     4, 6, 7,
            //     // left
            //     16,17,18,
            //     16,18,19,
            //     // right
            //     21,20,22,
            //     22,20,23,
            // };

            // tile_effect_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 24, 1);
            // save_mesh_to_c_file(tile_effect_mesh, "tile_effect_mesh", "./game/models/tile_effect_mesh.c");
            bind_mesh(tile_effect_mesh);
            // }
        // <tile_effect_mesh>

        // <rect_plane_mesh>
            rect_plane_mesh = generate_2d_quad_mesh(0, 1, 0, 1, 0, 1, 0, 1);
        // </rect_plane_mesh>

        // <field tile mesh>
            // field_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/field.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.5,
            //             .y = 0,
            //             .z = 0.5
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(field_tile_mesh, "field_tile_mesh", "./game/models/field_tile_mesh.c");
            bind_mesh(field_tile_mesh);

            // field_wheat_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/field_wheat.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.5,
            //             .y = 0,
            //             .z = 0.5
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(field_wheat_tile_mesh, "field_wheat_tile_mesh", "./game/models/field_wheat_tile_mesh.c");
            bind_mesh(field_wheat_tile_mesh);
        // </field tile mesh>

        // <forest tile mesh>
            // forest_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/forest.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.5,
            //             .y = 0,
            //             .z = 0.5
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(forest_tile_mesh, "forest_tile_mesh", "./game/models/forest_tile_mesh.c");
            bind_mesh(forest_tile_mesh);

            // forest_tree_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/tree.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.5,
            //             .y = 0,
            //             .z = 0.5
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(forest_tree_tile_mesh, "forest_tree_tile_mesh", "./game/models/forest_tree_tile_mesh.c");
            bind_mesh(forest_tree_tile_mesh);
        // </forest tile mesh>

        // <mine_tile_mesh>
            // mine_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/mine.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.5,
            //             .y = 0,
            //             .z = 0.5
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(mine_tile_mesh, "mine_tile_mesh", "./game/models/mine_tile_mesh.c");
            bind_mesh(mine_tile_mesh);
        // </mine_tile_mesh>
        
        // <house_tile_mesh>
            // house_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/house.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.4,
            //             .y = 0,
            //             .z = 0.35
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -M_PI*1.6, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(house_tile_mesh, "house_tile_mesh", "./game/models/house_tile_mesh.c");
            bind_mesh(house_tile_mesh);
        // </house_tile_mesh>
        
        // <barracks_tile_mesh>
            // barracks_tile_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/barracks.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0.55,
            //             .y = 0,
            //             .z = 0.55
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -M_PI*0.05, -0),
            //         .scale = (vec3_t){
            //             .x = 0.167,
            //             .y = 0.167,
            //             .z = 0.167
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(barracks_tile_mesh, "barracks_tile_mesh", "./game/models/barracks_tile_mesh.c");
            bind_mesh(barracks_tile_mesh);
        // </barracks_tile_mesh>

        // <scale_base_mesh>
            // scale_base_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/scale_base.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0,
            //             .y = 0,
            //             .z = 0
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.5,
            //             .y = 0.5,
            //             .z = 0.5
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(scale_base_mesh, "scale_base_mesh", "./game/models/scale_base_mesh.c");
            bind_mesh(scale_base_mesh);
        // </scale_base_mesh>

        // <scale_head_mesh>
            // scale_head_mesh = mesh_from_wavefront_obj_ext(
            //     "./game/models/scale_head.obj",
            //     (quat_vec_vec_t){
            //         .pos = (vec3_t){
            //             .x = 0,
            //             .y = 0,
            //             .z = 0
            //         },
            //         .rot = quat_from_axis_angles_yzx(-0, -0, -0),
            //         .scale = (vec3_t){
            //             .x = 0.5,
            //             .y = 0.5,
            //             .z = 0.5
            //         }
            //     },
            //     1
            // );
            // save_mesh_to_c_file(scale_head_mesh, "scale_head_mesh", "./game/models/scale_head_mesh.c");
            bind_mesh(scale_head_mesh);
        // </scale_head_mesh>
    // </meshes>

    // <cameras>
        camera = create_camera(
            0, 0, 0,
            M_PI*1.8, -M_PI*0.15, 0,
            380, 260, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );

        ui_camera = create_camera(
            _OUTPORT_WIDTH_*0.5, _OUTPORT_HEIGHT_*0.5, 0,
            0, 0, 0,
            _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );
        ui_camera->active = 0;
    // </cameras>

    // <sun shadow map>
        sun_shadow_map_camera = create_camera(
            0, 0, 0,
            M_PI*1.8, M_PI*1.1, 0,
            1080, 1080, 1600,
            -32000, 32000,
            0, 45,
            0, 0, 3240, 3240
        );
    // </sun shadow map>

    // <fbos>
        outport_fbo = create_fbo(_OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1, GL_RGB, 4);

        sun_shadow_map_fbo = create_fbo(3240, 3240, 0, 0, 2);
    // </fbos>

    // <textures>
        // floor_texture = load_texture("./game/textures/floor.png");
        // global_texture = load_texture("./game/textures/global_texture.png");
        // tile_texture = load_texture("./game/textures/tile_texture.png");
        // field_tile_texture = load_texture("./game/textures/field_tile_texture.png");
        // forest_tile_texture = load_texture("./game/textures/forest_tile_texture.png");
        // mine_tile_texture = load_texture("./game/textures/mine_tile_texture.png");
        // house_tile_texture = load_texture("./game/textures/house_tile_texture.png");
        // barracks_tile_texture = load_texture("./game/textures/barracks_tile_texture.png");
        // attack_effect_texture = load_texture("./game/textures/attack_effect.png");

        // letters_font_texture = load_texture("./game/textures/font.png");
        // big_letters_font_texture = load_texture("./game/textures/font_big.png");
        // nine_slice1_texture = load_texture("./game/textures/nslice1.png");;
        // nine_slice2_texture = load_texture("./game/textures/nslice2.png");
        // nine_slice3_texture = load_texture("./game/textures/nslice3.png");
        // menu_background_scroll_texture = load_texture("./game/textures/menu_background_scroll.png");

        // save_surface_to_c_file("./game/textures/floor.png", "floor_texture_surface", "./game/textures/floor_texture_surface.c");
        // save_surface_to_c_file("./game/textures/global_texture.png", "global_texture_surface", "./game/textures/global_texture_surface.c");
        // save_surface_to_c_file("./game/textures/tile_texture.png", "tile_texture_surface", "./game/textures/tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/field_tile_texture.png", "field_tile_texture_surface", "./game/textures/field_tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/forest_tile_texture.png", "forest_tile_texture_surface", "./game/textures/forest_tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/mine_tile_texture.png", "mine_tile_texture_surface", "./game/textures/mine_tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/house_tile_texture.png", "house_tile_texture_surface", "./game/textures/house_tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/barracks_tile_texture.png", "barracks_tile_texture_surface", "./game/textures/barracks_tile_texture_surface.c");
        // save_surface_to_c_file("./game/textures/attack_effect.png", "attack_effect_texture_surface", "./game/textures/attack_effect_texture_surface.c");

        // save_surface_to_c_file("./game/textures/font.png", "letters_font_texture_surface", "./game/textures/letters_font_texture_surface.c");
        // save_surface_to_c_file("./game/textures/font_big.png", "big_letters_font_texture_surface", "./game/textures/big_letters_font_texture_surface.c");
        // save_surface_to_c_file("./game/textures/nslice1.png", "nine_slice1_texture_surface", "./game/textures/nine_slice1_texture_surface.c");
        // save_surface_to_c_file("./game/textures/nslice2.png", "nine_slice2_texture_surface", "./game/textures/nine_slice2_texture_surface.c");
        // save_surface_to_c_file("./game/textures/nslice3.png", "nine_slice3_texture_surface", "./game/textures/nine_slice3_texture_surface.c");
        // save_surface_to_c_file("./game/textures/menu_background_scroll.png", "menu_background_scroll_texture_surface", "./game/textures/menu_background_scroll_texture_surface.c");
        

        floor_texture = load_texture_from_surface(floor_texture_surface);
        global_texture = load_texture_from_surface(global_texture_surface);
        tile_texture = load_texture_from_surface(tile_texture_surface);
        field_tile_texture = load_texture_from_surface(field_tile_texture_surface);
        forest_tile_texture = load_texture_from_surface(forest_tile_texture_surface);
        mine_tile_texture = load_texture_from_surface(mine_tile_texture_surface);
        house_tile_texture = load_texture_from_surface(house_tile_texture_surface);
        barracks_tile_texture = load_texture_from_surface(barracks_tile_texture_surface);
        attack_effect_texture = load_texture_from_surface(attack_effect_texture_surface);

        letters_font_texture = load_texture_from_surface(letters_font_texture_surface);
        big_letters_font_texture = load_texture_from_surface(big_letters_font_texture_surface);
        nine_slice1_texture = load_texture_from_surface(nine_slice1_texture_surface);
        nine_slice2_texture = load_texture_from_surface(nine_slice2_texture_surface);
        nine_slice3_texture = load_texture_from_surface(nine_slice3_texture_surface);
        menu_background_scroll_texture = load_texture_from_surface(menu_background_scroll_texture_surface);


        // <letters_font>
            letters_font = (font_t){
                .font_texture = letters_font_texture,
                .letters_in_row = 16,
                .letters_in_col = 10,
                .letter_width = 6,
                .letter_height = 12
            };
        // </letters_font>

        // <big_letters_font>
            big_letters_font = (font_t){
                .font_texture = big_letters_font_texture,
                .letters_in_row = 16,
                .letters_in_col = 10,
                .letter_width = 12,
                .letter_height = 24
            };
        // </big_letters_font>

        // <nine_slices>
            nine_slice1 = (nine_slice_t){
                .texture_width = 16,
                .texture_height = 16,
                .padding = 3,
                .texture = nine_slice1_texture
            };
            
            nine_slice2 = (nine_slice_t){
                .texture_width = 16,
                .texture_height = 16,
                .padding = 3,
                .texture = nine_slice2_texture
            };
            
            nine_slice3 = (nine_slice_t){
                .texture_width = 16,
                .texture_height = 16,
                .padding = 3,
                .texture = nine_slice3_texture
            };
        // </nine_slices>
        

    // </textures>

    // <shaders>
        // global_shader = create_shader_from_files(
        //     "./game/shaders/global.vert",
        //     "./game/shaders/global.frag",
        //     "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        //     "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
        // );

        // ui_shader = create_shader_from_files(
        //     "./game/shaders/ui.vert",
        //     "./game/shaders/ui.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale", 2
        // );

        // nine_slice_shader = create_shader_from_files(
        //     "./game/shaders/nine_slice.vert",
        //     "./game/shaders/nine_slice.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_texcoord_change", 3
        // );

        // font_shader = create_shader_from_files(
        //     "./game/shaders/font.vert",
        //     "./game/shaders/font.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_quat_rotation\0u_text_row_length\0u_text_row\0u_font_data\0u_color", 7
        // );

        // cooldown_billboards_shader = create_shader_from_files(
        //     "./game/shaders/cooldown.vert",
        //     "./game/shaders/cooldown.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_rads", 3
        // );

        // attacked_billboards_shader = create_shader_from_files(
        //     "./game/shaders/attacked.vert",
        //     "./game/shaders/attacked.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_frames_amount\0u_current_frame", 4
        // );

        // tile_effect_shader = create_shader_from_files(
        //     "./game/shaders/tile_effect.vert",
        //     "./game/shaders/tile_effect.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time\0u_width\0u_length", 9
        // );

        // build_preview_shader = create_shader_from_files(
        //     "./game/shaders/build_preview.vert",
        //     "./game/shaders/build_preview.frag",
        //     "in_vertex_position\0in_vertex_texcoord", 2,
        //     "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time", 7
        // );
        
        // sun_shadow_map_shader = create_shader_from_files(
        //     "./game/shaders/sun_shadow_map.vert",
        //     "./game/shaders/sun_shadow_map.frag",
        //     "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        //     "u_position\0u_scale\0u_quat_rotation", 3
        // );

        global_shader = create_shader(
            (const char**)&global_vert_sh_p,
            (const char**)&global_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
            "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
        );

        ui_shader = create_shader(
            (const char**)&ui_vert_sh_p,
            (const char**)&ui_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale", 2
        );

        nine_slice_shader = create_shader(
            (const char**)&nine_slice_vert_sh_p,
            (const char**)&nine_slice_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_texcoord_change", 3
        );

        font_shader = create_shader(
            (const char**)&font_vert_sh_p,
            (const char**)&font_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation\0u_text_row_length\0u_text_row\0u_font_data\0u_color", 7
        );

        cooldown_billboards_shader = create_shader(
            (const char**)&cooldown_vert_sh_p,
            (const char**)&cooldown_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_rads", 3
        );

        attacked_billboards_shader = create_shader(
            (const char**)&attacked_vert_sh_p,
            (const char**)&attacked_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_frames_amount\0u_current_frame", 4
        );

        tile_effect_shader = create_shader(
            (const char**)&tile_effect_vert_sh_p,
            (const char**)&tile_effect_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time\0u_width\0u_length", 9
        );

        build_preview_shader = create_shader(
            (const char**)&build_preview_vert_sh_p,
            (const char**)&build_preview_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time", 7
        );
        
        sun_shadow_map_shader = create_shader(
            (const char**)&sun_shadow_map_vert_sh_p,
            (const char**)&sun_shadow_map_frag_sh_p,
            "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
            "u_position\0u_scale\0u_quat_rotation", 3
        );
    // </shaders>

    // <sounds>
        #ifdef DEBUG_SOFT_MODE
        // build_tile_sound = audio_sound_load("./game/sounds/place_tile.wav");        // \x   // X
        // attack_tile_sound = audio_sound_load("./game/sounds/attack_tile.wav");      // X    // X
        // demolish_tile_sound = audio_sound_load("./game/sounds/demolish_tile.wav");  // X    // X
        // wheight_up_sound = audio_sound_load("./game/sounds/wheight_up.wav");        // X    // X
        // wheight_down_sound = audio_sound_load("./game/sounds/wheight_down.wav");    // X    // X
        // resource_give_sound = audio_sound_load("./game/sounds/resource_give.wav");  // X    // X
        // win_game_sound = audio_sound_load("./game/sounds/win_game.wav");            // /x   // X
        // lose_game_sound = audio_sound_load("./game/sounds/lose_game.wav");          // /x   // X
        // switch_turn_sound = audio_sound_load("./game/sounds/switch_turn.wav");      // X    // X
        // button_press_sound = audio_sound_load("./game/sounds/button_press.wav");    // X    // X
        // error_sound = audio_sound_load("./game/sounds/error.wav");                  // X    // X
        // select_tile_sound = audio_sound_load("./game/sounds/select_tile.wav");      // X    // X
        #endif

        // audio_save_sound_to_c_file(build_tile_sound, "build_tile_sound", "./game/sounds/build_tile_sound.c");
        // audio_save_sound_to_c_file(attack_tile_sound, "attack_tile_sound", "./game/sounds/attack_tile_sound.c");
        // audio_save_sound_to_c_file(demolish_tile_sound, "demolish_tile_sound", "./game/sounds/demolish_tile_sound.c");
        // audio_save_sound_to_c_file(wheight_up_sound, "wheight_up_sound", "./game/sounds/wheight_up_sound.c");
        // audio_save_sound_to_c_file(wheight_down_sound, "wheight_down_sound", "./game/sounds/wheight_down_sound.c");
        // audio_save_sound_to_c_file(resource_give_sound, "resource_give_sound", "./game/sounds/resource_give_sound.c");
        // audio_save_sound_to_c_file(win_game_sound, "win_game_sound", "./game/sounds/win_game_sound.c");
        // audio_save_sound_to_c_file(lose_game_sound, "lose_game_sound", "./game/sounds/lose_game_sound.c");
        // audio_save_sound_to_c_file(switch_turn_sound, "switch_turn_sound", "./game/sounds/switch_turn_sound.c");
        // audio_save_sound_to_c_file(button_press_sound, "button_press_sound", "./game/sounds/button_press_sound.c");
        // audio_save_sound_to_c_file(error_sound, "error_sound", "./game/sounds/error_sound.c");
        // audio_save_sound_to_c_file(select_tile_sound, "select_tile_sound", "./game/sounds/select_tile_sound.c");

        talking_mud_music = audio_music_load("./game/sounds/talking_mud.flac");
        if (talking_mud_music == NULL) {
            talking_mud_music = audio_music_load("./src/sounds/talking_mud.flac");
        }
        if (talking_mud_music != NULL) {
            audio_music_play(talking_mud_music);
        }
    // </sounds>

    // <tile_type_properties>
        tile_type_properties[TILE_TYPE_HOUSE] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 2,
                .wheat = 1,
                .population = 0,
                .soldiers = 0
            },
            .give_cooldown = 3,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 0,
                .population = 1,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_BARRACKS] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 2,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 3,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 0,
                .population = 0,
                .soldiers = 1
            }
        };
        tile_type_properties[TILE_TYPE_FIELD] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 0,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 1,
                .population = 0,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_MINE] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 0,
                .stone = 1,
                .wheat = 0,
                .population = 0,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_FOREST] = (tile_type_t){
            .cost = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 0,
                .population = 0,
                .soldiers = 0
            }
        };

        // build_info_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char build_info_string[_TILE_BUILD_INFO_STRING_MAX_LENGTH_];
            uint32_t c = 0;

            
            if (tile_type_properties[i].give_cooldown == 1) {
                strcpy(&(build_info_string[c]), "Every turn:\n");
                c += strlen("Every turn:\n");
            }else {
                build_info_string[c  ]  = 'E';
                build_info_string[c+1]  = 'v';
                build_info_string[c+2]  = 'e';
                build_info_string[c+3]  = 'r';
                build_info_string[c+4]  = 'y';
                build_info_string[c+5]  = ' ';
                build_info_string[c+6]  = '0' + tile_type_properties[i].give_cooldown;
                build_info_string[c+7]  = ' ';
                build_info_string[c+8]  = 't';
                build_info_string[c+9]  = 'u';
                build_info_string[c+10] = 'r';
                build_info_string[c+11] = 'n';
                build_info_string[c+12] = 's';
                build_info_string[c+13] = ':';
                build_info_string[c+14] = '\n';
                c += 15;
            }
            
            if (tile_type_properties[i].give.population > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.population;
                build_info_string[c+4] = '\x11';
                build_info_string[c+5] = '\x12';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                build_info_string[c+4] = '\x13';
                build_info_string[c+5] = '\x14';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                build_info_string[c+4] = '\x15';
                build_info_string[c+5] = '\x16';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                build_info_string[c+4] = '\x17';
                build_info_string[c+5] = '\x18';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                build_info_string[c+4] = '\x19';
                build_info_string[c+5] = '\x1a';
                build_info_string[c+6] = '\n';
                c += 7;
            }

            build_info_string[c] = '\n';
            c += 1;

            strcpy(&(build_info_string[c]), "Costs:\n");
            c += strlen("costs:\n");
            
            if (tile_type_properties[i].cost.population > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.population;
                build_info_string[c+4] = '\x11';
                build_info_string[c+5] = '\x12';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wheat;
                build_info_string[c+4] = '\x13';
                build_info_string[c+5] = '\x14';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wood;
                build_info_string[c+4] = '\x15';
                build_info_string[c+5] = '\x16';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.stone;
                build_info_string[c+4] = '\x17';
                build_info_string[c+5] = '\x18';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.soldiers;
                build_info_string[c+4] = '\x19';
                build_info_string[c+5] = '\x1a';
                build_info_string[c+6] = '\n';
                c += 7;
            }

            // remove newline and add null terminator
            build_info_string[c-1] = '\0';

            strcpy(tile_type_properties[i].build_info_string, build_info_string);
        }
        
        // give_alert_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char give_alert_string[_TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_];
            uint32_t c = 0;

            if (tile_type_properties[i].give.population > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.population;
                give_alert_string[c+2] = '\x11';
                give_alert_string[c+3] = '\x12';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wheat;
                give_alert_string[c+2] = '\x13';
                give_alert_string[c+3] = '\x14';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.wood > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wood;
                give_alert_string[c+2] = '\x15';
                give_alert_string[c+3] = '\x16';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.stone > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.stone;
                give_alert_string[c+2] = '\x17';
                give_alert_string[c+3] = '\x18';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.soldiers;
                give_alert_string[c+2] = '\x19';
                give_alert_string[c+3] = '\x1a';
                give_alert_string[c+4] = '\n';
                c += 5;
            }

            // remove newline and add null terminator
            give_alert_string[c-1] = '\0';

            strcpy(tile_type_properties[i].give_alert_string, give_alert_string);
        }

        // demolish_info_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char demolish_info_string[_TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_];
            uint32_t c = 0;
            
            strcpy(&(demolish_info_string[c]), "Destroy this building\n");
            c += strlen("Destroy this building\n");

            if (tile_type_properties[i].give.population > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.population;
                demolish_info_string[c+4] = '\x11';
                demolish_info_string[c+5] = '\x12';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                demolish_info_string[c+4] = '\x13';
                demolish_info_string[c+5] = '\x14';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wood > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                demolish_info_string[c+4] = '\x15';
                demolish_info_string[c+5] = '\x16';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.stone > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                demolish_info_string[c+4] = '\x17';
                demolish_info_string[c+5] = '\x18';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                demolish_info_string[c+4] = '\x19';
                demolish_info_string[c+5] = '\x1a';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }

            // remove newline and add null terminator
            demolish_info_string[c-1] = '\0';

            strcpy(tile_type_properties[i].demolish_info_string, demolish_info_string);
        }
    // </tile_type_properties>


    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].active = 0;
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;

    enter_main_menu();

    return 0;
}