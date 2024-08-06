#include "engine_types.h"


uint32_t engine_init();
void engine_handle_event();
void engine_update();
void engine_render();
void engine_clean();

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
                        2 = engine_render buffer object
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
                        4 = engine_render buffer object
                        5 = engine_render buffer object, only depth
                        6 = engine_render buffer object, only stencil
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

// <mesh>
    mesh_t* generate_mesh(vbo_data_t* vbo_datas_arr, uint32_t vbo_datas_arr_size, uint32_t* indices_array, uint32_t indices_count, uint8_t unbinded);
    void bind_mesh(mesh_t* mesh);
    mesh_t* generate_2d_quad_mesh( float min_x, float max_x, float min_y, float max_y,
                                    float min_u, float max_u, float min_v, float max_v);
    mesh_t* mesh_generate_ball(uint32_t divisions);
    mesh_t* mesh_from_wavefront_obj_ext(const char* obj_file_path, quat_vec_vec_t transform_qvv, uint8_t unbinded);
    mesh_t* mesh_from_wavefront_obj(const char* obj_file_path, uint8_t unbinded);
    mesh_t* mesh_from_collada_dae_ext(const char* dae_file_path, quat_vec_vec_t transform_qvv, uint8_t unbinded);
    mesh_t* mesh_from_collada_dae(const char* dae_file_path, uint8_t unbinded);
    animation_t* animation_from_collada_dae_ext(const char* dae_file_path, joint_t* joints, uint32_t joints_amount, quat_vec_vec_t transform_qvv);
    animation_t* animation_from_collada_dae(const char* dae_file_path, joint_t* joints, uint32_t joints_amount);
    void draw_mesh_instanced(mesh_t* mesh, uint32_t instance_count);
    void draw_mesh(mesh_t* mesh);
    void draw_mesh_posed_instanced(mesh_t* mesh, uint32_t instance_count);
    void draw_mesh_posed(mesh_t* mesh);
    // mix: between 0 and 1; 0 = no effect; 1 = overides current animation
    void pose_mesh_mix_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp, float mix);
    void pose_mesh_set_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp);
    void destroy_mesh(mesh_t* mesh);
    void destroy_animation(animation_t* anim);
    void clean_mesh(mesh_t* mesh);
    void clean_animation(animation_t* anim);
// </mesh>