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
    int32_t intmin(int32_t a, int32_t b);
    int32_t intmax(int32_t a, int32_t b);
    int64_t int64min(int64_t a, int64_t b);
    int64_t int64max(int64_t a, int64_t b);
    uint64_t load_file_contents(char** load_to, const char* file_path);
    int64_t str_find_substr(char* str, char* substr);
    // assumes str is numbers with spaces in between
    // array needs to be freed at the end of use
    float* str_to_float_array(char* str, uint64_t arr_size);
    // assumes str is numbers with spaces in between
    // array needs to be freed at the end of use
    int32_t* str_to_int_array(char* str, uint64_t arr_size);
    // assumes str is numbers with spaces in between
    // array needs to be bigger/equal to floats_amount
    void str_to_existing_float_array(char* str, uint64_t floats_amount, float* arr);
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
    mesh_t* mesh_from_wavefront_obj_ext(const char* obj_file_path, quat_vec_vec_t transform_qvv);
    mesh_t* mesh_from_wavefront_obj(const char* obj_file_path);
    mesh_t* mesh_from_collada_dae_ext(const char* dae_file_path, quat_vec_vec_t transform_qvv);
    mesh_t* mesh_from_collada_dae(const char* dae_file_path);
    void draw_mesh(mesh_t* mesh);
    void draw_instanced_mesh(mesh_t* mesh, uint32_t instance_count);
    void destroy_mesh(mesh_t* mesh);
    void clean_mesh(mesh_t* mesh);
    animation_t* animation_from_collada_dae_ext(const char* dae_file_path, joint_t* joints, uint32_t joints_amount, quat_vec_vec_t transform_qvv);
    animation_t* animation_from_collada_dae(const char* dae_file_path, joint_t* joints, uint32_t joints_amount);
    void draw_mesh_animated(mesh_t* mesh, animation_t* anim, float time_stamp);
    void destroy_animation(animation_t* anim);
    void clean_animation(animation_t* anim);
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

// <matrices vectors and quaternions>
    vec3_t vec3_mul(vec3_t vec1, vec3_t vec2);
    vec3_t vec3_add(vec3_t vec1, vec3_t vec2);
    float dot_product(vec3_t vec1, vec3_t vec2);
    vec3_t cross_product(vec3_t vec1, vec3_t vec2);

    mat4_t mat4_from_mat3(mat3_t mat);
    mat3_t mat3_from_mat4(mat4_t mat);
    //  assumes:
    //      mat1 width  = mat2 height = m
    //      mat3 width  = mat2 width  = n
    //      mat3 height = mat1 height = i
    void mat_mul(float* mat1, float* mat2, float* restrict mat3, uint8_t m, uint8_t i, uint8_t n);
    mat3_t mat3_mul(mat3_t mat1, mat3_t mat2);
    mat4_t mat4_mul(mat4_t mat1, mat4_t mat2);
    vec3_t vec3_mul_by_mat3(vec3_t vec, mat3_t mat);
    mat3_t mat3_mul_by_scalar(mat3_t mat, float scalar);
    mat4_t mat4_mul_by_scalar(mat4_t mat, float scalar);
    //  assumes:
    //      mat1 width  = mat2 width  = mat3 width  = w
    //      mat1 height = mat3 height = mat3 height = h 
    void mat_add(float* mat1, float* mat2, float* mat3, uint8_t w, uint8_t h);
    mat3_t mat3_add(mat3_t mat1, mat3_t mat2);
    mat4_t mat4_add(mat4_t mat1, mat4_t mat2);
    // mat must be of size 3x3
    mat3_t mat3_from_axis_angle(vec3_t axis, float ang);

    quat_t quat_multiply(quat_t q1, quat_t q2);
    quat_t quat_conjugate(quat_t q);
    quat_t quat_slerp(quat_t q1, quat_t q2, float t);
    quat_t quat_from_axis_angle(vec3_t vec, float ang);
    quat_t quat_from_euler_angles(float roll, float pitch, float yaw);
    quat_t quat_from_axis_angles_yzx(float rx, float ry, float rz);
    vec3_t rotate_vector(vec3_t vec, quat_t q);
    vec3_t vec_scale_rotate_translate(vec3_t vec, quat_vec_vec_t qvv);
    mat3_t rot_mat3_from_quat(quat_t q);
    quat_t quat_from_rot_mat3(mat3_t mat);
    quat_vec_vec_t quat_vec_vec_from_mat4(mat4_t mat);
    mat4_t mat4_from_quat_vec_vec(quat_vec_vec_t qvv);

    void print_mat3(mat3_t mat);
    void print_mat4(mat4_t mat);
    void print_quat_vec_vec(quat_vec_vec_t qvv);
// </matrices vectors and quaternions>

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