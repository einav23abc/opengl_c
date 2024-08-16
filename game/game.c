#include "game.h"


// variable declarations

fbo_t* outport_fbo;

texture_t* global_texture;
shader_t* global_shader;

vec3_t camera_pos;
camera_t* camera;

mesh_t* cube_mesh;

game_t game_struct;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;
shader_t* sun_shadow_map_shader;


vec3_t get_mouse_world_space_position_at_y(float at_y) {
    // assuming using `camera`-ortho and `outport_fbo`
    // very unoptimised, quickly hacked together

    vec3_t mouse_relative_to_middle = {
        .x =  (((float)mouse.x)/window_drawable_width  - 0.5)*camera->width,
        .y = -(((float)mouse.y)/window_drawable_height - 0.5)*camera->height,
        .z = 0
    };

    quat_t camera_rot_quat = quat_from_axis_angles_yzx(-camera->rx, -camera->ry, -camera->rz);
    vec3_t mouse_as_camera_plane_translation = rotate_vector(mouse_relative_to_middle, camera_rot_quat);

    vec3_t pos = {
        .x = camera_pos.x + mouse_as_camera_plane_translation.x,
        .y = camera_pos.y + mouse_as_camera_plane_translation.y,
        .z = camera_pos.z + mouse_as_camera_plane_translation.z
    };

    vec3_t camera_in_translation = rotate_vector((vec3_t){0,0,1}, camera_rot_quat);

    // find intersection with top of player[0].tiles
    float t = (at_y-pos.y)/camera_in_translation.y;
    pos = vec3_add(pos, vec3_mul_by_scalar(camera_in_translation, t));
    
    return pos;
}