#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"


#define _OUTPORT_WIDTH_ (380*2)
#define _OUTPORT_HEIGHT_ (260*2)

#define _OUTPORT_BACKGROUND_COLOR_R_ (71.0/255)
#define _OUTPORT_BACKGROUND_COLOR_G_ (65.0/255)
#define _OUTPORT_BACKGROUND_COLOR_B_ (107.0/255)


#define _PLAYER_GRID_WIDTH_ (5)
#define _PLAYER_GRID_DEPTH_ (5)
#define _TILE_SIZE_ (16)
#define _PLAYER_CONSTANT_Z_TRANSLATION_ (-_PLAYER_GRID_DEPTH_*_TILE_SIZE_*0.5)
#define _PLAYER_TRANSLATION_LERP_DURATION_ (500)
#define _PLAYER_COOLDOWN_TRANSLATION_LERP_DURATION_ (500)
#define _WIN_WHEIGHT_ (8)
#define _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_ (2)
#define _SCALE_AXIS_POINT_Y_ (_TILE_SIZE_*(_WIN_WHEIGHT_*0.5))
#define _SCALE_AXIS_LENGTH_ (sqrt(_SCALE_AXIS_POINT_Y_*_SCALE_AXIS_POINT_Y_ + _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_TILE_SIZE_*_TILE_SIZE_))

#define _MAX_TEXT_ROW_LENGTH (64)




typedef struct {
    int32_t wood;
    int32_t stone;
    int32_t wheat;
    int32_t population;
    int32_t soldiers;
} resources_t;

#define _TILE_BUILD_INFO_STRING_MAX_LENGTH_ (64)
#define _TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_ (64)
#define _TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_ (64)
typedef struct {
    resources_t cost;
    int32_t give_cooldown;
    resources_t give;
    char build_info_string[_TILE_BUILD_INFO_STRING_MAX_LENGTH_];
    char give_alert_string[_TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_];
    char demolish_info_string[_TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_];
} tile_type_t;

enum TILE_TYPES {
    TILE_TYPE_EMPTY = -1,
    TILE_TYPE_HOUSE = 0,
    TILE_TYPE_BARRACKS = 1,
    TILE_TYPE_FIELD = 2,
    TILE_TYPE_MINE = 3,
    TILE_TYPE_FOREST = 4
};
// excluding TILE_TYPE_EMPTY
#define _TILE_TYPES_AMOUNT_ (5)

typedef struct {
    int32_t type;
    float curent_cooldown_timer;
    int32_t cooldown_timer;
    uint8_t camoflauged : 1;
} tile_t;

typedef struct {
    float y_lerp_start_translation;
    float y_translation;
    float y_current_translation;
    float x_current_translation;
    float hinge_y_position;
    float hinge_x_position;
    uint32_t translation_lerp_time;
    
    int32_t wheight;
    resources_t resources;
    tile_t tiles[_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_];
} player_t;

typedef struct {
    player_t players[2];
    uint8_t player_turn;
    uint8_t game_ended;
} game_t;


typedef struct {
    int32_t letters_in_row;
    int32_t letters_in_col;
    int32_t letter_width;
    int32_t letter_height;
    texture_t* font_texture;
} font_t;

typedef struct {
    int32_t texture_width;
    int32_t texture_height;
    int32_t padding;
    texture_t* texture;
} nine_slice_t;



enum PAGES {
    PAGE_IN_GAME,
    PAGE_MAIN_MENU
};


int32_t page;

extern fbo_t* outport_fbo;

extern texture_t* floor_texture;
extern texture_t* global_texture;

extern nine_slice_t nine_slice1;
extern nine_slice_t nine_slice2;
extern nine_slice_t nine_slice3;

extern font_t letters_font;
extern font_t big_letters_font;

extern shader_t* global_shader;
extern shader_t* ui_shader;
extern shader_t* nine_slice_shader;
extern shader_t* font_shader;
extern shader_t* cooldown_billboards_shader;
extern shader_t* tile_effect_shader;
extern shader_t* build_preview_shader;

extern vec3_t camera_pos;
extern camera_t* camera;
extern camera_t* ui_camera;

extern mesh_t* rect_plane_mesh;
extern mesh_t* cube_mesh;
extern mesh_t* centered_cube_mesh;
extern mesh_t* tile_effect_mesh;

extern tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

extern game_t game_struct;
extern ivec3_t selected_tile;
extern ivec2_t hovered_tiles[2];
extern int8_t in_cooldowns_translation;
extern int8_t in_tiles_translation;
extern int8_t player1_ai_played;

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;
extern shader_t* sun_shadow_map_shader;



void mouse_press_in_game();

void render_main_menu();

void update_game();
void player_translations_update();
void tile_cooldowns_update();
void update_hovered_tile();
void camera_update();
void sun_shadow_map_update();

void render_game();
void draw_tile(int32_t player_i, int32_t tile_x, int32_t tile_z, int32_t tile_type);
void render_game_world();
void render_game_effects();
void render_game_ui();

// returns the drawn width
float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g);
uvec2_t get_str_boxed_size(char* str, float row_height);
void draw_str_boxed(char* str, font_t font, nine_slice_t nslice, uint32_t left_x, uint32_t bottom_y, uint32_t padding, uint32_t row_height);
void draw_nine_slice(nine_slice_t nine_slice, int32_t x, int32_t y, int32_t width, int32_t height);

vec2_t outport_space_position_from_world_space(vec3_t pos);
vec2_t get_mouse_outport_space_position();
vec2_t get_mouse_camera_space_position();
vec3_t get_mouse_world_space_position_at_y(float at_y);
ivec2_t get_hovered_tile_position(uint8_t player_i);

int32_t has_enough_resources(int32_t player_id, int32_t tile_type_id);

void enter_main_menu();
void enter_game();
void switch_turn_button_callback(int32_t ui_list_id, int32_t button_id);
void request_switch_turn();
void switch_turn();
void player_1_turn();
void player_1_ai_turn();

void ui_list_exit_game_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_play_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_build_specific_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_build_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_attack_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_demolish_sure_button_callback(int32_t ui_list_id, int32_t button_id);
void ui_list_demolish_button_callback(int32_t ui_list_id, int32_t button_id);


#include "billboard.h"
#include "ui_lists.h"
#include "alerts.h"

#endif