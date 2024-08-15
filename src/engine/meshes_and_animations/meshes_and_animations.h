#ifndef MESHES_AND_ANIMATIONS_H
#define MESHES_AND_ANIMATIONS_H

#include <SDL2/SDL.h>
#include "../glad/glad.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../vec_mat_quat/vec_mat_quat.h"
#include "../shaders/shaders.h"


struct vbo_data_t;
struct joint_t;
struct mesh_t;
struct key_frame_t;
struct animation_t;


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
    const int64_t mesh_index;

    /* 0 = not saved.
     * non-zero = saved.
     *
     * See `print_mesh_for_saving`.
     */
    uint8_t saved;

    uint8_t unbinded; // 0 = binded ; non-zero = unbinded
    
    uint32_t vao;
    uint32_t vbos_amount;
    uint32_t* vbos;
    vbo_data_t* vbo_datas_arr; // only relevent when unbinded
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



extern uint64_t meshes_amount;
extern mesh_t* meshes_list[];
extern const uint64_t MESHES_MAX_AMOUNT;

extern uint64_t animations_amount;
extern animation_t* animations_list[];
extern const uint64_t ANIMATIONS_MAX_AMOUNT;



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

// mix: between 0 and 1; 0 = no effect; 1 = overides current animation
void pose_mesh_mix_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp, float mix);
void pose_mesh_set_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp);

void draw_mesh_instanced(mesh_t* mesh, uint32_t instance_count);
void draw_mesh(mesh_t* mesh);
void draw_mesh_posed_instanced(mesh_t* mesh, uint32_t instance_count);
void draw_mesh_posed(mesh_t* mesh);

/* Destroys a mesh. The mesh will not be usable after destruction
 * \param mesh the mesh to destroy.
*/
void destroy_mesh(mesh_t* mesh);
/* Destroys an animation. The animation will not be usable after destruction
 * \param anim the animation to destroy.
*/
void destroy_animation(animation_t* anim);

/* Called by the engine when exiting.
 * Destroys all created meshes.
*/
void clean_meshes();
/* Called by the engine when exiting.
 * Destroys all created animations.
*/
void clean_animations();


/* \brief Print a mesh_t struct for saving.
 *
 * Can be used to load a saved mesh without having to load from a file.
 * 
 * Binded meshes will not be printed beacuse they will not work if saved.
 * This is beacuse the mesh must be binded at each runtime.
 * 
 * Saved meshes do not have an index beacuse they dont need to be deallocated.
 * Beacuse of this, Using `destroy_mesh` on a saved mesh will have undefined behavior.
 * 
 * \param mesh The mesh that will be printed.
 */
void print_mesh_for_saving(mesh_t* mesh);

#endif