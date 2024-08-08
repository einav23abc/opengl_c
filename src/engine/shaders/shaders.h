#ifndef SHADERS_H
#define SHADERS_H

#include <SDL2/SDL.h>
#include "../glad/glad.h"
#include <stdint.h>
#include <stdio.h>

#include "../utils/utils.h"
#include "../cameras/cameras.h"


typedef struct {
    const uint64_t shader_index;
    
    uint8_t active : 1;

    uint32_t program;
    uint32_t vert_shader;
    uint32_t frag_shader;

    // <uniforms>
    int32_t u_texture_loc;
    int32_t u_camera_world_view_projection_matrix_loc;

    int32_t u_joint_matrices_loc;

    int32_t u_instanced_drawing_float_data_loc;
    int32_t u_instanced_drawing_uint_data_loc;

    int32_t* uniform_locations;
    // </uniforms>

    // the camera index the u_camera_world_view_projection_matrix_loc uniform was last updated by
    int64_t wvp_mat_camera_index;
} shader_t;
#define u_camera_wvp_mat_loc u_camera_world_view_projection_matrix_loc


extern uint64_t shaders_amount;
extern int64_t current_shader;
extern shader_t* shaders_list[];
extern const uint64_t SHADERS_MAX_AMOUNT;


/* Creates a shader.
 * 
 * The memory delocation of this shader is handled by the engine (by calling `clean_shaders()`).
 *
 * \param vert_shader_str A pointer to the vertex shader string.
 * \param frag_shader_str A pointer to the fragment shader string.
 * 
 * \param attribute_names A string with all the shader's attribute names, Seperated by `\0`. For example: `"in_vertex_position\0in_vertex_texcoord\0in_vertex_normal"`.
 * \param attributes_count The amount of attributes in `attribute_names`.
 * \param uniform_names A string with all the shader's uniform names, Seperated by `\0`. For example: `"u_position\0u_scale\0u_quat_rotation"`.
 * \param uniforms_count The amount of uniforms in `uniform_names`.
 * 
 * \returns A pointer to an allocated shader or `NULL` on failure.
*/
shader_t* create_shader(    const char** vert_shader_str, const char** frag_shader_str,
                            const char* attribute_names , uint32_t attributes_count,
                            const char* uniform_names, uint32_t uniforms_count);

/* Creates a shader.
 * 
 * The memory delocation of this shader is handled by the engine (by calling `clean_shaders()`).
 *
 * \param vert_shader_file_path A string of the vertex shader file's path.
 * \param frag_shader_file_path A string of the fragment shader file's path.
 * 
 * \param attribute_names A string with all the shader's attribute names, Seperated by `\0`. For example: `"in_vertex_position\0in_vertex_texcoord\0in_vertex_normal"`.
 * \param attributes_count The amount of attributes in `attribute_names`.
 * \param uniform_names A string with all the shader's uniform names, Seperated by `\0`. For example: `"u_position\0u_scale\0u_quat_rotation"`.
 * \param uniforms_count The amount of uniforms in `uniform_names`.
 * 
 * \returns A pointer to an allocated shader or `NULL` on failure.
*/
shader_t* create_shader_from_files( const char* vert_shader_file_path, const char* frag_shader_file_path,
                                    const char* attribute_names , uint32_t attributes_count,
                                    const char* uniform_names, uint32_t uniforms_count);
/* Sets the `current_shader` and uses it for all draw functions until a diffrent shader is used.
 *
 * Updates the shader's `u_camera_world_view_projection_matrix` uniform by calling `update_shader_camera_uniforms()`,
 * Unless that uniform is already set to the current camera, Indicated by the `shader`'s `wvp_mat_camera_index`.
 * 
 * \param shader The shader to use, if the `shader`'s index is `current_shader` the function has no effect.
*/
void use_shader(shader_t* shader);
/* Sets the `current_camera`'s `world_view_projection_matrix` to the `current_shader`'s `u_camera_world_view_projection_matrix` uniform.
 * 
 * Gets called automaticly by `use_shader()` and `camera_use()`, so generally there is no reason to call this function.
*/
void update_shader_camera_uniforms();
/* Destroys a shader.
 *
 * The shader will not be usable after destruction.
 *
 * Will not destroy the `current_shader`, if you wish to do so, call `use_shader()` with a differnt shader first.
 * \param shader the shader to destroy.
 * \returns `0` on success or `-1` on failure
*/
int32_t destroy_shader(shader_t* shader);
/* Called by the engine when exiting.
 * Destroys all created shaders.
*/
void clean_shaders();

#endif
