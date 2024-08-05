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
    float w;
    float x;
    float y;
    float z;
} quat_t;

typedef struct {
    quat_t rot;
    vec3_t pos;
    vec3_t scale;
} quat_vec_vec_t;

typedef struct {
    float mat[9];
} mat3_t;

typedef struct {
    float mat[16];
} mat4_t;

struct vbo_data_t;
struct joint_t;
struct mesh_t;
struct key_frame_t;
struct animation_t;

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

typedef struct joint_t {
    uint32_t index;
    char* name;
    uint32_t parent;
    mat4_t inverse_bind_transform_mat;

    // set and used by the animatoring functions
    mat4_t model_transform_mat;
} joint_t;

typedef struct mesh_t {
    const uint64_t mesh_index;

    uint8_t unbinded; // 0 = binded ; non-zero = unbinded
    vbo_data_t* vbo_datas_arr; // only relevent when unbinded
    
    uint32_t vao;
    uint32_t vbos_amount;
    uint32_t* vbos;
    uint32_t indices_count;
    uint32_t* indices_array;
    
    uint32_t joints_amount;
    joint_t* joints;
} mesh_t;

typedef struct key_frame_t {
    mat4_t joint_local_transform;
    quat_vec_vec_t joint_local_transform_qvv;
    float time_stamp;
} key_frame_t;

typedef struct joint_key_frame_t {
    uint32_t key_frames_amount;
    key_frame_t* key_frames;
} joint_key_frame_t;

typedef struct animation_t {
    const uint64_t animation_index;

    uint32_t joints_amount;
    joint_key_frame_t* joints_key_frames;
} animation_t;


typedef struct shader_t {
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

    mat4_t world_view_projection_matrix;
} camera_t;
#define wvp_mat world_view_projection_matrix


#endif