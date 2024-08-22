#include "meshes.h"

#ifdef RESAVE_MESHES
mesh_t* rect_plane_mesh;
mesh_t* cube_mesh;
mesh_t* centered_cube_mesh;
mesh_t* hinge_mesh;
mesh_t* tile_effect_mesh;
mesh_t* field_tile_mesh;
mesh_t* field_wheat_tile_mesh;
mesh_t* forest_tile_mesh;
mesh_t* forest_tree_tile_mesh;
mesh_t* mine_tile_mesh;
mesh_t* house_tile_mesh;
mesh_t* barracks_tile_mesh;
mesh_t* scale_base_mesh;
mesh_t* scale_head_mesh;
#else
#include "../models/rect_plane_mesh.c"
#include "../models/cube_mesh.c"
#include "../models/centered_cube_mesh.c"
#include "../models/hinge_mesh.c"
#include "../models/tile_effect_mesh.c"
#include "../models/field_tile_mesh.c"
#include "../models/field_wheat_tile_mesh.c"
#include "../models/forest_tile_mesh.c"
#include "../models/forest_tree_tile_mesh.c"
#include "../models/mine_tile_mesh.c"
#include "../models/house_tile_mesh.c"
#include "../models/barracks_tile_mesh.c"
#include "../models/scale_base_mesh.c"
#include "../models/scale_head_mesh.c"
#endif

void game_meshes_init() {
    #ifdef RESAVE_MESHES
        // <rect_plane_mesh>
            rect_plane_mesh = generate_2d_quad_mesh(0, 1, 0, 1, 0, 1, 0, 1, 1);
            save_mesh_to_c_file(rect_plane_mesh, "rect_plane_mesh", "./game/models/rect_plane_mesh.c");
        // </rect_plane_mesh>
    
        // <cube mesh & centered cube mesh>
            if(1){
            float vertices_position_arr[] = {
                // front
                0,0,0,
                0,1,0,
                1,1,0,
                1,0,0,
                // back
                0,0,1,
                0,1,1,
                1,1,1,
                1,0,1,
                // bottom
                0,0,0,
                0,0,1,
                1,0,1,
                1,0,0,
                // top
                0,1,0,
                0,1,1,
                1,1,1,
                1,1,0,
                // left
                0,0,0,
                0,1,0,
                0,1,1,
                0,0,1,
                // right
                1,0,0,
                1,1,0,
                1,1,1,
                1,0,1,
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
            save_mesh_to_c_file(cube_mesh, "cube_mesh", "./game/models/cube_mesh.c");
            
            for (uint32_t i = 0; i <sizeof(vertices_position_arr)/sizeof(float); i++) vertices_position_arr[i] -= 0.5;
            centered_cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 1);
            save_mesh_to_c_file(centered_cube_mesh, "centered_cube_mesh", "./game/models/centered_cube_mesh.c");
            };
        // </cube mesh & centered cube mesh>

        // <hinge_mesh>
            hinge_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/hinge.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0,
                        .y = 0,
                        .z = 0
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.5,
                        .y = 0.5,
                        .z = 0.5
                    }
                },
                1
            );
            save_mesh_to_c_file(hinge_mesh, "hinge_mesh", "./game/models/hinge_mesh.c");
        // </hinge_mesh>

        // <tile_effect_mesh>
            if(1){
            float vertices_position_arr[] = {
                // front
                0,0,0,
                0,1,0,
                1,1,0,
                1,0,0,
                // back
                0,0,1,
                0,1,1,
                1,1,1,
                1,0,1,
                // bottom
                0,0,0,
                0,0,1,
                1,0,1,
                1,0,0,
                // top
                0,1,0,
                0,1,1,
                1,1,1,
                1,1,0,
                // left
                0,0,0,
                0,1,0,
                0,1,1,
                0,0,1,
                // right
                1,0,0,
                1,1,0,
                1,1,1,
                1,0,1,
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
                // left
                16,17,18,
                16,18,19,
                // right
                21,20,22,
                22,20,23,
            };

            tile_effect_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 24, 1);
            save_mesh_to_c_file(tile_effect_mesh, "tile_effect_mesh", "./game/models/tile_effect_mesh.c");
            }
        // <tile_effect_mesh>

        // <field tile mesh>
            field_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/field.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.5,
                        .y = 0,
                        .z = 0.5
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(field_tile_mesh, "field_tile_mesh", "./game/models/field_tile_mesh.c");

            field_wheat_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/field_wheat.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.5,
                        .y = 0,
                        .z = 0.5
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(field_wheat_tile_mesh, "field_wheat_tile_mesh", "./game/models/field_wheat_tile_mesh.c");
        // </field tile mesh>

        // <forest tile mesh>
            forest_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/forest.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.5,
                        .y = 0,
                        .z = 0.5
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(forest_tile_mesh, "forest_tile_mesh", "./game/models/forest_tile_mesh.c");

            forest_tree_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/tree.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.5,
                        .y = 0,
                        .z = 0.5
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(forest_tree_tile_mesh, "forest_tree_tile_mesh", "./game/models/forest_tree_tile_mesh.c");
        // </forest tile mesh>

        // <mine_tile_mesh>
            mine_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/mine.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.5,
                        .y = 0,
                        .z = 0.5
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(mine_tile_mesh, "mine_tile_mesh", "./game/models/mine_tile_mesh.c");
        // </mine_tile_mesh>
        
        // <house_tile_mesh>
            house_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/house.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.4,
                        .y = 0,
                        .z = 0.35
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -M_PI*1.6, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(house_tile_mesh, "house_tile_mesh", "./game/models/house_tile_mesh.c");
        // </house_tile_mesh>
        
        // <barracks_tile_mesh>
            barracks_tile_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/barracks.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0.55,
                        .y = 0,
                        .z = 0.55
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -M_PI*0.05, -0),
                    .scale = (vec3_t){
                        .x = 0.167,
                        .y = 0.167,
                        .z = 0.167
                    }
                },
                1
            );
            save_mesh_to_c_file(barracks_tile_mesh, "barracks_tile_mesh", "./game/models/barracks_tile_mesh.c");
        // </barracks_tile_mesh>

        // <scale_base_mesh>
            scale_base_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/scale_base.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0,
                        .y = 0,
                        .z = 0
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.5,
                        .y = 0.5,
                        .z = 0.5
                    }
                },
                1
            );
            save_mesh_to_c_file(scale_base_mesh, "scale_base_mesh", "./game/models/scale_base_mesh.c");
        // </scale_base_mesh>

        // <scale_head_mesh>
            scale_head_mesh = mesh_from_wavefront_obj_ext(
                "./game/models/scale_head.obj",
                (quat_vec_vec_t){
                    .pos = (vec3_t){
                        .x = 0,
                        .y = 0,
                        .z = 0
                    },
                    .rot = quat_from_axis_angles_yzx(-0, -0, -0),
                    .scale = (vec3_t){
                        .x = 0.5,
                        .y = 0.5,
                        .z = 0.5
                    }
                },
                1
            );
            save_mesh_to_c_file(scale_head_mesh, "scale_head_mesh", "./game/models/scale_head_mesh.c");
        // </scale_head_mesh>
    #endif

    bind_mesh(rect_plane_mesh);
    bind_mesh(cube_mesh);
    bind_mesh(centered_cube_mesh);
    bind_mesh(hinge_mesh);
    bind_mesh(tile_effect_mesh);
    bind_mesh(field_tile_mesh);
    bind_mesh(field_wheat_tile_mesh);
    bind_mesh(forest_tile_mesh);
    bind_mesh(forest_tree_tile_mesh);
    bind_mesh(mine_tile_mesh);
    bind_mesh(house_tile_mesh);
    bind_mesh(barracks_tile_mesh);
    bind_mesh(scale_base_mesh);
    bind_mesh(scale_head_mesh);
}