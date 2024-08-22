#include "game.h"

#include "pages/how_to_play/page.h"
#include "pages/in_game/page.h"
#include "pages/main_menu/page.h"

// variable declarations

page_t pages[PAGES_AMOUNT] = {
    [PAGE_IN_GAME] = (page_t){
        .init = &init_in_game,
        .enter = &enter_in_game,
        .exit = NULL,
        .update = &update_in_game,
        .render = &render_in_game,
        .mouse_press = &mouse_press_in_game
    },
    [PAGE_MAIN_MENU] = (page_t){
        .init = NULL,
        .enter = &enter_main_menu,
        .exit = NULL,
        .update = NULL,
        .render = &render_main_menu,
        .mouse_press = NULL
    },
    [PAGE_HOW_TO_PLAY] = (page_t){
        .init = NULL,
        .enter = &enter_how_to_play,
        .exit = NULL,
        .update = NULL,
        .render = &render_how_to_play,
        .mouse_press = NULL
    }
};
int32_t page = -1;

fbo_t* outport_fbo;

nine_slice_t nine_slice1;
nine_slice_t nine_slice2;
nine_slice_t nine_slice3;

font_t letters_font;
font_t big_letters_font;

vec3_t camera_pos;
camera_t* camera;
camera_t* ui_camera;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;


void switch_page(int32_t page_i) {
    if (page_i < 0 || page_i >= PAGES_AMOUNT) return;
    if (page >= 0 && page < PAGES_AMOUNT) {
        if (pages[page].exit != NULL) pages[page].exit();
    }
    if (pages[page_i].enter != NULL) pages[page_i].enter();
    page = page_i;
}


vec2_t outport_space_position_from_world_space(fbo_t* fbo, camera_t* camera, vec3_t pos) {
    // for `camera`
    vec2_t osp_pos = vec2_from_vec4(vec4_mul_by_mat4(vec4(pos.x,pos.y,pos.z,1.0), camera->world_view_projection_matrix));
    return vec2_mul(
        vec2_add(vec2_mul_by_scalar(osp_pos, 0.5),vec2(0.5,0.5)),
        vec2(
            fbo->width,
            _OUTPORT_HEIGHT_
        )
    );
}
vec2_t get_mouse_outport_space_position(fbo_t* fbo) {
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
vec2_t get_mouse_camera_space_position(fbo_t* fbo, camera_t* camera) {
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
vec3_t get_mouse_world_space_position_at_y(fbo_t* fbo, camera_t* camera, float at_y) {
    // assuming using `camera`-ortho and `outport_fbo`
    // very unoptimised, quickly hacked together

    vec3_t mouse_camera_space_position = vec3_from_vec2(get_mouse_camera_space_position(fbo, camera));

    quat_t camera_rot_quat = quat_from_axis_angles_yzx(-camera->rx, -camera->ry, -camera->rz);
    vec3_t mouse_as_camera_plane_translation = rotate_vector(mouse_camera_space_position, camera_rot_quat);

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
