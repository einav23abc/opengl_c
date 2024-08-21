#include "simple_draw_module.h"


float simple_draw_module_color_r;
float simple_draw_module_color_g;
float simple_draw_module_color_b;
float simple_draw_module_color_a;
const shader_t* simple_draw_module_cube_shader;
const shader_t* simple_draw_module_screen_quad_mesh_shader;
#ifdef RESAVE_MESHES
mesh_t* simple_draw_module_rectangle_mesh;
mesh_t* simple_draw_module_cube_mesh;
mesh_t* simple_draw_module_ball_mesh;
mesh_t* simple_draw_module_screen_quad_mesh;
#else
#include "./simple_draw_module_rectangle_mesh.c"
#include "./simple_draw_module_cube_mesh.c"
#include "./simple_draw_module_ball_mesh.c"
#include "./simple_draw_module_screen_quad_mesh.c"
#endif


int32_t simple_draw_module_init() {
    simple_draw_module_set_color(1, 1, 1, 1);

    // <simple_draw_module_cube_shader>
        #include "cube_sh.c"
        
        simple_draw_module_cube_shader = create_shader(
            (const char**)&cube_vert,
            (const char**)&cube_frag,
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_size\0u_color", 3
        );
        if (simple_draw_module_cube_shader == NULL) {
            printf("failed to create simple_draw_module_cube_shader\n");
            return 1;
        }
    // </simple_draw_module_cube_shader>

    // <simple_draw_module_rectangle_mesh>
        if(1){
        float vertices_position_arr[] = {
            0,0,0,
            0,1,0,
            1,1,0,
            1,0,0
        };

        vbo_data_t vbo_datas_arr[1] = {
            {
                .data_arr_size = sizeof(vertices_position_arr),
                .data_arr = (void*)vertices_position_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            }
        };

        uint32_t indices_array[] = {
            0, 1, 2,
            0, 2, 3
        };

        #ifdef RESAVE_MESHES
        simple_draw_module_rectangle_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6, 1);
        if (simple_draw_module_rectangle_mesh == NULL) {
            printf("failed to generate simple_draw_module_rectangle_mesh\n");
            return 1;
        }
        save_mesh_to_c_file(simple_draw_module_rectangle_mesh, "simple_draw_module_rectangle_mesh", "./engine/simple_draw_module/simple_draw_module_rectangle_mesh.c");
        #endif
        bind_mesh(simple_draw_module_rectangle_mesh);
        }
    // </simple_draw_module_rectangle_mesh>

    // <simple_draw_module_cube_mesh>
        if(1){
        float vertices_position_arr[] = {
            0,0,0,
            1,0,0,
            0,1,0,
            1,1,0,

            0,0,1,
            1,0,1,
            0,1,1,
            1,1,1
        };

        vbo_data_t vbo_datas_arr[1] = {
            {
                .data_arr_size = sizeof(vertices_position_arr),
                .data_arr = (void*)vertices_position_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            }
        };

        uint32_t indices_array[] = {
            //Top
            2, 6, 7,
            2, 3, 7,
            //Bottom
            0, 4, 5,
            0, 1, 5,
            //Left
            0, 2, 6,
            0, 4, 6,
            //Right
            1, 3, 7,
            1, 5, 7,
            //Front
            0, 2, 3,
            0, 1, 3,
            //Back
            4, 6, 7,
            4, 5, 7
        };

        #ifdef RESAVE_MESHES
        simple_draw_module_cube_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6*6, 1);
        if (simple_draw_module_cube_mesh == NULL) {
            printf("failed to generate simple_draw_module_cube_mesh\n");
            return 1;
        }
        save_mesh_to_c_file(simple_draw_module_cube_mesh, "simple_draw_module_cube_mesh", "./engine/simple_draw_module/simple_draw_module_cube_mesh.c");
        #endif
        bind_mesh(simple_draw_module_cube_mesh);
        }
    // </simple_draw_module_cube_mesh>

    // <simple_draw_module_ball_mesh>
        #ifdef RESAVE_MESHES
        simple_draw_module_ball_mesh = mesh_generate_ball(4, 1);
        if (simple_draw_module_ball_mesh == NULL) {
            printf("failed to generate simple_draw_module_ball_mesh\n");
            return 1;
        }
        save_mesh_to_c_file(simple_draw_module_ball_mesh, "simple_draw_module_ball_mesh", "./engine/simple_draw_module/simple_draw_module_ball_mesh.c");
        #endif
        bind_mesh(simple_draw_module_ball_mesh);
    // </simple_draw_module_ball_mesh>

    // <simple_draw_module_screen_quad_mesh_shader>
        #include "screen_quad_mesh_sh.c"
        
        // used along with screen_quad_mesh to draw a texture to a section of the screen
        simple_draw_module_screen_quad_mesh_shader = create_shader(
            (const char**)&screen_quad_mesh_vert,
            (const char**)&screen_quad_mesh_frag,
            "in_vertex_data", 1,
            "", 0
        );
        if (simple_draw_module_screen_quad_mesh_shader == NULL) {
            printf("failed to create simple_draw_module_screen_quad_mesh_shader\n");
            return 1;
        }
    // </simple_draw_module_screen_quad_mesh_shader>

    // <simple_draw_module_screen_quad_mesh>
        // used along with simple_draw_module_screen_quad_mesh_shader to draw a texture to a section of the screen
        if(1){
        float vertices_data_arr[] = {
            0,0,
            0,1,
            1,1,
            1,0
        };

        vbo_data_t vbo_datas_arr[1] = {
            {
                .data_arr_size = sizeof(vertices_data_arr),
                .data_arr = (void*)vertices_data_arr,
                .size = 2,
                .type = GL_FLOAT,
                .stride = 2*sizeof(float),
                .divisor = 0
            }
        };

        uint32_t indices_array[] = {
            1, 0, 2,
            2, 0, 3
        };

        #ifdef RESAVE_MESHES
        simple_draw_module_screen_quad_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6, 1);
        if (simple_draw_module_screen_quad_mesh == NULL) {
            printf("failed to generate simple_draw_module_screen_quad_mesh\n");
            return 1;
        }
        save_mesh_to_c_file(simple_draw_module_screen_quad_mesh, "simple_draw_module_screen_quad_mesh", "./engine/simple_draw_module/simple_draw_module_screen_quad_mesh.c");
        #endif
        bind_mesh(simple_draw_module_screen_quad_mesh);
        }
    // </simple_draw_module_screen_quad_mesh>

    return 0;
}

void simple_draw_module_set_color(float r, float g, float b, float a) {
    simple_draw_module_color_r = r;
    simple_draw_module_color_g = g;
    simple_draw_module_color_b = b;
    simple_draw_module_color_a = a;
    return;
}
void simple_draw_module_draw_rect(float x, float y, float w, float h) {
    int64_t last_shader = current_shader;

    uint32_t cull_face_was_enabled = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    use_shader((shader_t*)simple_draw_module_cube_shader);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, 0);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], w, h, 0);
    glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
        simple_draw_module_color_r,
        simple_draw_module_color_g,
        simple_draw_module_color_b,
        simple_draw_module_color_a
    );
    draw_mesh((mesh_t*)simple_draw_module_rectangle_mesh);

    if (cull_face_was_enabled) glEnable(GL_CULL_FACE);

    use_shader(shaders_list[last_shader]);
    return;
}
void simple_draw_module_draw_cube(float x, float y, float z, float w, float h, float d) {
    int64_t last_shader = current_shader;

    uint32_t cull_face_was_enabled = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    use_shader((shader_t*)simple_draw_module_cube_shader);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, z);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], w, h, d);
    glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
        simple_draw_module_color_r,
        simple_draw_module_color_g,
        simple_draw_module_color_b,
        simple_draw_module_color_a
    );
    draw_mesh((mesh_t*)simple_draw_module_cube_mesh);

    if (cull_face_was_enabled) glEnable(GL_CULL_FACE);

    use_shader(shaders_list[last_shader]);
    return;
}
void simple_draw_module_draw_ball(float x, float y, float z, float r) {
    int64_t last_shader = current_shader;

    uint32_t cull_face_was_enabled = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    use_shader((shader_t*)simple_draw_module_cube_shader);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, z);
    glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], r, r, r);
    glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
        simple_draw_module_color_r,
        simple_draw_module_color_g,
        simple_draw_module_color_b,
        simple_draw_module_color_a
    );
    draw_mesh((mesh_t*)simple_draw_module_ball_mesh);

    if (cull_face_was_enabled) glEnable(GL_CULL_FACE);

    use_shader(shaders_list[last_shader]);
    return;
}

void simple_draw_module_draw_texture(texture_t* texture) {
    uint32_t depth_test_was_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    use_shader((shader_t*)simple_draw_module_screen_quad_mesh_shader);
    bind_texture(texture, simple_draw_module_screen_quad_mesh_shader->u_texture_loc, 0);
    draw_mesh((mesh_t*)simple_draw_module_screen_quad_mesh);
    if(depth_test_was_enabled) glEnable(GL_DEPTH_TEST);
}

void simple_draw_module_draw_fbo_color_texture(fbo_t* fbo) {
    uint32_t depth_test_was_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    use_shader((shader_t*)simple_draw_module_screen_quad_mesh_shader);
    bind_fbo_color_texture(fbo, simple_draw_module_screen_quad_mesh_shader->u_texture_loc, 0);
    draw_mesh((mesh_t*)simple_draw_module_screen_quad_mesh);
    if(depth_test_was_enabled) glEnable(GL_DEPTH_TEST);
}

void simple_draw_module_draw_fbo_depth_stencil_texture(fbo_t* fbo) {
    uint32_t depth_test_was_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    use_shader((shader_t*)simple_draw_module_screen_quad_mesh_shader);
    bind_fbo_depth_stencil_texture(fbo, simple_draw_module_screen_quad_mesh_shader->u_texture_loc, 0);
    draw_mesh((mesh_t*)simple_draw_module_screen_quad_mesh);
    if(depth_test_was_enabled) glEnable(GL_DEPTH_TEST);
}