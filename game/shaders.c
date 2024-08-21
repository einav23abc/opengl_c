#include "shaders.h"


#ifndef SHADERS_FROM_FILES
#include "./shaders/font_sh.c"
#include "./shaders/nine_slice_sh.c"
#include "./shaders/global_sh.c"
#include "./shaders/ui_sh.c"
#include "./shaders/cooldown_sh.c"
#include "./shaders/attacked_sh.c"
#include "./shaders/tile_effect_sh.c"
#include "./shaders/build_preview_sh.c"
#include "./shaders/sun_shadow_map_sh.c"
static char* font_vert_sh_p = font_vert_sh;
static char* font_frag_sh_p = font_frag_sh;
static char* nine_slice_vert_sh_p = nine_slice_vert_sh;
static char* nine_slice_frag_sh_p = nine_slice_frag_sh;
static char* global_vert_sh_p = global_vert_sh;
static char* global_frag_sh_p = global_frag_sh;
static char* ui_vert_sh_p = ui_vert_sh;
static char* ui_frag_sh_p = ui_frag_sh;
static char* cooldown_vert_sh_p = cooldown_vert_sh;
static char* cooldown_frag_sh_p = cooldown_frag_sh;
static char* attacked_vert_sh_p = attacked_vert_sh;
static char* attacked_frag_sh_p = attacked_frag_sh;
static char* tile_effect_vert_sh_p = tile_effect_vert_sh;
static char* tile_effect_frag_sh_p = tile_effect_frag_sh;
static char* build_preview_vert_sh_p = build_preview_vert_sh;
static char* build_preview_frag_sh_p = build_preview_frag_sh;
static char* sun_shadow_map_vert_sh_p = sun_shadow_map_vert_sh;
static char* sun_shadow_map_frag_sh_p = sun_shadow_map_frag_sh;
#endif
shader_t* font_shader;
shader_t* nine_slice_shader;
shader_t* global_shader;
shader_t* ui_shader;
shader_t* cooldown_billboards_shader;
shader_t* attacked_billboards_shader;
shader_t* tile_effect_shader;
shader_t* build_preview_shader;
shader_t* sun_shadow_map_shader;

void game_shaders_init() {
    #ifdef SHADERS_FROM_FILES
    font_shader = create_shader_from_files(
        "./game/shaders/font.vert",
        "./game/shaders/font.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_text_row_length\0u_text_row\0u_font_data\0u_color", 7
    );

    nine_slice_shader = create_shader_from_files(
        "./game/shaders/nine_slice.vert",
        "./game/shaders/nine_slice.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_texcoord_change", 3
    );

    global_shader = create_shader_from_files(
        "./game/shaders/global.vert",
        "./game/shaders/global.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
    );

    ui_shader = create_shader_from_files(
        "./game/shaders/ui.vert",
        "./game/shaders/ui.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale", 2
    );

    cooldown_billboards_shader = create_shader_from_files(
        "./game/shaders/cooldown.vert",
        "./game/shaders/cooldown.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_rads", 3
    );

    attacked_billboards_shader = create_shader_from_files(
        "./game/shaders/attacked.vert",
        "./game/shaders/attacked.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_frames_amount\0u_current_frame", 4
    );

    tile_effect_shader = create_shader_from_files(
        "./game/shaders/tile_effect.vert",
        "./game/shaders/tile_effect.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time\0u_width\0u_length", 9
    );

    build_preview_shader = create_shader_from_files(
        "./game/shaders/build_preview.vert",
        "./game/shaders/build_preview.frag",
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time", 7
    );
    
    sun_shadow_map_shader = create_shader_from_files(
        "./game/shaders/sun_shadow_map.vert",
        "./game/shaders/sun_shadow_map.frag",
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation", 3
    );
    #else
    font_shader = create_shader(
        (const char**)&font_vert_sh_p,
        (const char**)&font_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_text_row_length\0u_text_row\0u_font_data\0u_color", 7
    );

    nine_slice_shader = create_shader(
        (const char**)&nine_slice_vert_sh_p,
        (const char**)&nine_slice_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_texcoord_change", 3
    );

    global_shader = create_shader(
        (const char**)&global_vert_sh_p,
        (const char**)&global_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
    );

    ui_shader = create_shader(
        (const char**)&ui_vert_sh_p,
        (const char**)&ui_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale", 2
    );

    cooldown_billboards_shader = create_shader(
        (const char**)&cooldown_vert_sh_p,
        (const char**)&cooldown_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_rads", 3
    );

    attacked_billboards_shader = create_shader(
        (const char**)&attacked_vert_sh_p,
        (const char**)&attacked_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_frames_amount\0u_current_frame", 4
    );

    tile_effect_shader = create_shader(
        (const char**)&tile_effect_vert_sh_p,
        (const char**)&tile_effect_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time\0u_width\0u_length", 9
    );

    build_preview_shader = create_shader(
        (const char**)&build_preview_vert_sh_p,
        (const char**)&build_preview_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time", 7
    );
    
    sun_shadow_map_shader = create_shader(
        (const char**)&sun_shadow_map_vert_sh_p,
        (const char**)&sun_shadow_map_frag_sh_p,
        "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
        "u_position\0u_scale\0u_quat_rotation", 3
    );
    #endif
}