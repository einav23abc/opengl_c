#ifndef SHADERS_H
#define SHADERS_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
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


shader_t* shader_create(    const char** vert_shader_str, const char** frag_shader_str,
                            const char* attribute_names , uint32_t attributes_count,
                            const char* uniform_names, uint32_t uniforms_count);
shader_t* shader_create_from_files( const char* vert_shader_file_path, const char* frag_shader_file_path,
                                    const char* attribute_names , uint32_t attributes_count,
                                    const char* uniform_names, uint32_t uniforms_count);
void shader_use(shader_t* shader);
void shader_update_camera_uniforms();
// * Will not destroy the `current_shader`. If you wish to do so, call `shader_use()` with a diffrent shader first.
// * \param shader the shader to destroy. This shader will not be usable after destruction.
// * \returns `0` on success or `-1` on failure
int32_t shader_destroy(shader_t* shader);
// * Should be called before exiting.
// * Destroys all created shaders.
void shaders_clean();

#endif