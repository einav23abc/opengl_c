#include "cameras.h"


#ifndef _CAMERAS_MAX_AMOUNT_
#define _CAMERAS_MAX_AMOUNT_ (128)
#endif
uint64_t cameras_amount = 0;
int64_t current_camera = -1;
camera_t* cameras_list[_CAMERAS_MAX_AMOUNT_];
const uint64_t CAMERAS_MAX_AMOUNT = _CAMERAS_MAX_AMOUNT_;


camera_t* create_camera(float x, float y, float z,
                        float rx, float ry, float rz,
                        float width, float height, float depth,
                        float near, float far,
                        uint8_t is_prespective, float fov,
                        float viewport_x, float viewport_y, float viewport_w, float viewport_h) {
    
    if (cameras_amount >= CAMERAS_MAX_AMOUNT) return NULL;

    camera_t* camera = malloc(sizeof(camera_t));
    if (camera == NULL) return NULL;


    camera->active = 1;

    camera->is_prespective = (is_prespective > 0);

    camera->x = x;
    camera->y = y;
    camera->z = z;

    camera->rx = rx;
    camera->ry = ry;
    camera->rz = rz;

    camera->width = width;
    camera->height = height;
    camera->depth = depth;

    camera->near = near;
    camera->far = far;
    camera->fov = fov;

    camera->viewport_x = viewport_x;
    camera->viewport_y = viewport_y;
    camera->viewport_w = viewport_w;
    camera->viewport_h = viewport_h;

    // append camera to cameras_list
    *((uint64_t*)(&camera->camera_index)) = cameras_amount;
    cameras_list[cameras_amount] = camera;
    cameras_amount += 1;

    return camera;
}
void use_camera(camera_t* camera) {
    current_camera = camera->camera_index;
    update_camera_fbo_viewport(camera);

    if (shaders_list[current_shader]->wvp_mat_camera_index == current_camera) return;
    update_shader_camera_uniforms();
    shaders_list[current_shader]->wvp_mat_camera_index = current_camera;
}
void update_camera_fbo_viewport(camera_t* camera) {
    glViewport(
        camera->viewport_x,
        camera->viewport_y,
        camera->viewport_w,
        camera->viewport_h
    );
}
void update_camera_world_view_projection_matrix(camera_t* camera) {
    #define cx camera->x
    #define cy camera->y
    #define cz camera->z
    #define crx camera->rx
    #define cry camera->ry
    #define crz camera->rz
    #define cn camera->near
    #define cf camera->far
    #define cw camera->width
    #define ch camera->height

    const mat4_t rotation_x_matrix = (mat4_t){
        .mat = {
            1, 0,         0,        0,
            0, cos(crx), -sin(crx), 0,
            0, sin(crx),  cos(crx), 0,
            0, 0,         0,        1
        }
    };
    const mat4_t rotation_y_matrix = (mat4_t){
        .mat = {
                cos(cry), 0, sin(cry), 0,
                0,        1, 0,        0,
            -sin(cry), 0, cos(cry), 0,
                0,        0, 0,        1
        }
    };
    const mat4_t rotation_z_matrix = (mat4_t){
        .mat = {
            cos(crz), -sin(crz), 0, 0,
            sin(crz),  cos(crz), 0, 0,
            0,         0,        1, 0,
            0,         0,        0, 1
        }
    };

    const mat4_t world_matrix = (mat4_t){
        .mat = {
            1, 0, 0, -cx,
            0, 1, 0, -cy,
            0, 0, 1, -cz,
            0, 0, 0, 1
        }
    };
    
    mat4_t projection_matrix;
    if (camera->is_prespective) {
        const float _s_ = 1/tan(camera->fov*M_PI*0.5/180);
        projection_matrix  = (mat4_t){
            .mat = {
                _s_*ch/(cw+ch), 0,              0,           0,
                0,              _s_*cw/(cw+ch), 0,           0,
                0,              0,              cf/(cf-cn), -cf*cn/(cf-cn),
                0,              0,              1,           0
            }
        };
    }else{
        projection_matrix  = (mat4_t){
            .mat = {
                2.0/cw, 0,      0,            0,
                0,      2.0/ch, 0,            0,
                0,      0,      2.0/(cf-cn), -(cf+cn)/(cf-cn),
                0,      0,      0,            1
            }
        };
    }
    
    // rotation_x_matrix
    // rotation_y_matrix
    // rotation_z_matrix
    
    // projection_matrix
    // world_matrix
    // view_matrix

    mat4_t view_matrix = mat4_mul(mat4_mul(rotation_z_matrix, rotation_x_matrix), rotation_y_matrix);

    camera->world_view_projection_matrix = mat4_mul(mat4_mul(projection_matrix, view_matrix), world_matrix);

    #undef cx
    #undef cy
    #undef cz
    #undef crx
    #undef cry
    #undef crz
    #undef cn
    #undef cf
    #undef cw
    #undef ch
    return;
}

static void clean_camera(camera_t* camera) {
    free(camera);
}
int32_t destoroy_camera(camera_t* camera) {
    if (current_camera != -1){
        #ifdef DEBUG_SOFT_MODE
        printf("cannot destroy a camera during render period!\n");
        #endif
        return -1;
    }

    // shader->wvp_mat_camera_index

    cameras_amount -= 1;

    // move the last camera at cameras_list to camera's index
    camera_t* last_cameras = cameras_list[cameras_amount];
    *((uint64_t*)&last_cameras->camera_index) = camera->camera_index;
    cameras_list[camera->camera_index] = last_cameras;

    clean_camera(camera);
    return 0;
}
void clean_cameras() {
    #ifdef DEBUG_SOFT_MODE
    printf("cleaning %u cameras\n", cameras_amount);
    #endif
    for (uint64_t i = 0; i < cameras_amount; i++) clean_camera(cameras_list[i]);
    cameras_amount = 0;
}