#include "spaces.h"

// was outport_space_position_from_world_space
vec2_t fbo_view_position_from_world_position(fbo_t* fbo, camera_t* camera, vec3_t pos) {
    // for `camera`
    vec2_t osp_pos = vec2_from_vec4(vec4_mul_by_mat4(vec4(pos.x,pos.y,pos.z,1.0), camera->world_view_projection_matrix));
    return vec2_mul(
        vec2_add(vec2_mul_by_scalar(osp_pos, 0.5),vec2(0.5,0.5)),
        vec2(
            fbo->width,
            fbo->height
        )
    );
}
// was get_mouse_outport_space_position
vec2_t fbo_view_position_from_mouse_position(fbo_t* fbo) {
    uint32_t pixel_scale = uintmin(window_drawable_width/fbo->width, window_drawable_height/fbo->height);
    uint32_t w = fbo->width*pixel_scale;
    uint32_t h = fbo->height*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/fbo->width, ((float)window_drawable_height)/fbo->height);
        w = fbo->width*fpixel_scale;
        h = fbo->height*fpixel_scale;
    }

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*fbo->width /w + fbo->width*0.5,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*fbo->height/h + fbo->height*0.5
    };
}
// was get_mouse_camera_space_position
vec2_t camera_view_position_from_mouse_position(fbo_t* fbo, camera_t* camera) {
    uint32_t pixel_scale = uintmin(window_drawable_width/fbo->width, window_drawable_height/fbo->height);
    uint32_t w = fbo->width*pixel_scale;
    uint32_t h = fbo->height*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/fbo->width, ((float)window_drawable_height)/fbo->height);
        w = fbo->width*fpixel_scale;
        h = fbo->height*fpixel_scale;
    }

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*camera->width /w,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*camera->height/h
    };
}
// was get_mouse_world_space_position_at_y
vec3_t world_position_at_y_from_mouse_position(fbo_t* fbo, camera_t* camera, float at_y) {
    // assuming using orthogonal camera
    // very unoptimised, quickly hacked together

    vec3_t mouse_camera_space_position = vec3_from_vec2(camera_view_position_from_mouse_position(fbo, camera));

    quat_t camera_rot_quat = quat_from_axis_angles_yzx(-camera->rx, -camera->ry, -camera->rz);
    vec3_t mouse_as_camera_plane_translation = rotate_vector(mouse_camera_space_position, camera_rot_quat);

    vec3_t pos = {
        .x = camera->x + mouse_as_camera_plane_translation.x,
        .y = camera->y + mouse_as_camera_plane_translation.y,
        .z = camera->z + mouse_as_camera_plane_translation.z
    };

    vec3_t camera_in_translation = rotate_vector((vec3_t){0,0,1}, camera_rot_quat);

    // find intersection with top of player[0].tiles
    float t = (at_y-pos.y)/camera_in_translation.y;
    pos = vec3_add(pos, vec3_mul_by_scalar(camera_in_translation, t));
    
    return pos;
}