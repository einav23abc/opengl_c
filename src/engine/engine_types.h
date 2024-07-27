#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    int32_t x;
    int32_t y;
} ivec2_t;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} ivec3_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} uvec2_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
} uvec3_t;

typedef struct {
    uvec2_t size; // {columns (width), rows (height)}
    float* mat;
} mat_t;

struct vbo_data_t;
struct mesh_t;

struct shader_t;

struct texture_parameter_t;

struct texture_t;

struct fbo_t;

struct camera_t;


typedef struct vbo_data_t {
    size_t data_arr_size;
    void* data_arr;
    int32_t size;
    GLenum type;
    int32_t stride;
    uint32_t divisor;
} vbo_data_t;

typedef struct mesh_t {
    const uint64_t mesh_index;
    
    uint32_t vao;
    uint32_t vbos_amount;
    uint32_t* vbos;
    uint32_t indices_count;
    uint32_t* indices_array;
} mesh_t;


typedef struct shader_t {
    const uint64_t shader_index;
    
    uint8_t active : 1;

    uint32_t program;
    uint32_t vert_shader;
    uint32_t frag_shader;

    // <uniforms>
        int32_t u_texture_loc;
        int32_t u_camera_world_view_projection_matrix_loc;

        int32_t u_instanced_drawing_float_data_loc;
        int32_t u_instanced_drawing_uint_data_loc;

        int32_t* uniform_locations;
    // <uniforms>

    // the camera index the u_camera_world_view_projection_matrix_loc uniform was last updated by
    int64_t wvp_mat_camera_index;
} shader_t;
#define u_camera_wvp_mat_loc u_camera_world_view_projection_matrix_loc


typedef struct texture_parameter_t {
    GLenum pname;
    GLint param;
} texture_parameter_t;


typedef struct texture_t {
    const uint64_t texture_index;

    uint32_t texture_id;
} texture_t;


typedef struct fbo_t {
    const uint64_t fbo_index;

    uint32_t fbo_id;

    uint32_t width;
    uint32_t height;

    // none/texture/rbo
    uint32_t color;

    // 0 = no color attachment
    // 1 = texture
    // 2 = render buffer object
    uint8_t color_status : 2;
    GLint color_format;

    // none/texture/rbo
    uint32_t depth_stencil;

    // 0 = no depth and stencil attachment
    // 1 = texture
    // 2 = texture, only depth
    // 3 = texture, only stencil
    // 4 = render buffer object
    // 5 = render buffer object, only depth
    // 6 = render buffer object, only stencil
    uint8_t depth_stencil_status : 3;
} fbo_t;


typedef struct camera_t {
    const uint64_t camera_index;

    uint8_t active : 1;

    uint8_t is_prespective : 1; // 0 -> orthographic , 1 -> prespective
    
    float x;
    float y;
    float z;
    
    float rx;
    float ry;
    float rz;

    float width;
    float height;
    float depth;

    float near;
    float far;
    float fov;

    float viewport_x;
    float viewport_y;
    float viewport_w;
    float viewport_h;

    mat_t world_view_projection_matrix;
} camera_t;
#define wvp_mat world_view_projection_matrix


#endif