#ifndef CAMERAS_H
#define CAMERAS_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>
#include <stdint.h>
#include "../vec_mat_quat/vec_mat_quat.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

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


extern uint64_t cameras_amount;
extern int64_t current_camera;
extern camera_t* cameras_list[];
extern const uint64_t CAMERAS_MAX_AMOUNT;


camera_t* camera_create(float x, float y, float z,
                        float rx, float ry, float rz,
                        float width, float height, float depth,
                        float near, float far,
                        uint8_t is_prespective, float fov,
                        float viewport_x, float viewport_y, float viewport_w, float viewport_h);
void camera_use(camera_t* camera);
void camera_update_fbo_viewport(camera_t* camera);
void camera_update_world_view_projection_matrix(camera_t* camera);
#define camera_update_wvp_mat camera_update_world_view_projection_matrix
// * Will not destroy the camera during render period (when `current_camera` is `-1`).
// * \param camera the camera to destroy. This camera will not be usable after destruction.
// * \returns `0` on success or `-1` on failure
int32_t camera_destroy(camera_t* camera);
// * Should be called before exiting.
// * Destroys all created cameras.
void cameras_clean();


#endif