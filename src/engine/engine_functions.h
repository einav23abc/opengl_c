#include "engine_types.h"

uint32_t init();
void handle_event();
void update();
void render();
void clean();

// <miscellaneous>
    uint32_t hash(uint32_t x);
    uint32_t uintmin(uint32_t a, uint32_t b);
    uint32_t uintmax(uint32_t a, uint32_t b);
    uint64_t load_file_contents(char** load_to, const char* file_path);
// </miscellaneous>

// <texture>
    texture_t* load_texture(const char* file_path);
    void bind_texture(texture_t* texture, int32_t uniform, uint8_t texture_num);
    void destroy_texture(texture_t* texture);
    void clean_texture(texture_t* texture);
// </texture>

// <frame buffer objects>
    fbo_t* create_fbo(  uint32_t width, uint32_t height,
                        /*
                        0 = no color attachment
                        1 = texture
                        2 = render buffer object
                        */
                        uint8_t color_status,
                        // GL_RGB, GL_RGBA, etc ; only relevent for texture color attachment
                        GLint color_format,
                        // irrelevent if color_status is not 1
                        texture_parameter_t* color_texture_parameters,
                        // irrelevent if color_status is not 1
                        uint32_t color_texture_parameters_count,
                        /*
                        0 = no depth and stencil attachment
                        1 = texture
                        2 = texture, only depth
                        3 = texture, only stencil
                        4 = render buffer object
                        5 = render buffer object, only depth
                        6 = render buffer object, only stencil
                        */
                        uint8_t depth_stencil_status,
                        // irrelevent if depth_stencil_status is not 1, 2 or 3
                        texture_parameter_t* depth_stencil_texture_parameters,
                        // irrelevent if depth_stencil_status is not 1, 2 or 3
                        uint32_t depth_stencil_texture_parameters_count);
    void use_fbo(fbo_t* fbo);
    void bind_fbo_color_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num);
    void bind_fbo_depth_stencil_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num);
    void destroy_fbo(fbo_t* fbo);
    void clean_fbo(fbo_t* fbo);
// </frame buffer objects>

// <shader>
    shader_t* shader_create(    const char** vert_shader_str, const char** frag_shader_str,
                                const char* attribute_names , uint32_t attributes_count,
                                const char* uniform_names, uint32_t uniforms_count);
    shader_t* shader_create_from_files( const char* vert_shader_file_path, const char* frag_shader_file_path,
                                        const char* attribute_names , uint32_t attributes_count,
                                        const char* uniform_names, uint32_t uniforms_count);
    void shader_use(shader_t* shader);
    void shader_update_camera_uniforms();
    void shader_destroy(shader_t* shader);
    void shader_clean(shader_t* shader);
// </shader>

// <mesh>
    mesh_t* generate_mesh(vbo_data_t* vbo_datas_arr, uint32_t vbo_datas_arr_size, uint32_t* indices_array, uint32_t indices_count);
    mesh_t* generate_2d_quad_mesh( float min_x, float max_x, float min_y, float max_y,
                                    float min_u, float max_u, float min_v, float max_v);
    mesh_t* mesh_generate_ball(uint32_t divisions);
    mesh_t* generate_mesh_from_wavefront_obj(const char* obj_file_path);
    void draw_with_mesh(mesh_t* mesh);
    void draw_instanced_with_mesh(mesh_t* mesh, uint32_t instance_count);
    void destroy_mesh(mesh_t* mesh);
    void clean_mesh(mesh_t* mesh);
// </mesh>

// <camera>
    camera_t* camera_create(    float x, float y, float z,
                                float rx, float ry, float rz,
                                float width, float height, float depth,
                                float near, float far,
                                uint8_t is_prespective, float fov,
                                float viewport_x, float viewport_y, float viewport_w, float viewport_h);
    void camera_use(camera_t* camera);
    void camera_update_fbo_viewport(camera_t* camera);
    void camera_destroy(camera_t* camera);
    void camera_clean(camera_t* camera);
    void camera_update_world_view_projection_matrix(camera_t* camera);
    #define camera_update_wvp_mat camera_update_world_view_projection_matrix
// </camera>

// <matrices>
    mat_t matrix_create(uvec2_t size);
    void matrix_multiply(mat_t* mat1, mat_t* mat2, mat_t* restrict mat3);
    void matrix_multiply_by_scalar(mat_t* mat1, float scalar, mat_t* mat2);
    void matrix_add(mat_t* mat1, mat_t* mat2, mat_t* mat3);
// </matrices>

// <simple draw module>
    void simple_draw_module_set_color(float r, float g, float b, float a);
    void simple_draw_module_draw_rect(float x, float y, float w, float h);
    void simple_draw_module_draw_cube(float x, float y, float z, float w, float h, float d);
    void simple_draw_module_draw_ball(float x, float y, float z, float r);
    #define sdm_set_color simple_draw_module_set_color
    #define sdm_draw_rect simple_draw_module_draw_rect
    #define sdm_draw_cube simple_draw_module_draw_cube
    #define sdm_draw_ball simple_draw_module_draw_ball
// </simple draw module>