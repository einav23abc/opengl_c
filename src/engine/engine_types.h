#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <stdint.h>
#include "vec_mat_quat/vec_mat_quat.h"


struct vbo_data_t;
struct joint_t;
struct mesh_t;
struct key_frame_t;
struct animation_t;

struct texture_parameter_t;

struct texture_t;

struct fbo_t;


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

    quat_vec_vec_t local_transform_qvv; // set and used by the drawing functions
    mat4_t model_transform_mat; // set and used by the drawing functions
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
    float* pose_joint_transform_matrices;
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



#endif