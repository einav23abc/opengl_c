#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"
#include "billboard.h"
#include "ui_lists.h"
#include "alerts.h"


#define _OUTPORT_WIDTH_ (380*2)
#define _OUTPORT_HEIGHT_ (260*2)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


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
typedef struct {
    resources_t cost;
    int32_t give_cooldown;
    resources_t give;
    char build_info_string[_TILE_BUILD_INFO_STRING_MAX_LENGTH_];
    char give_alert_string[_TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_];
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
} game_t;


typedef struct {
    int32_t letters_in_row;
    int32_t letters_in_col;
    int32_t letter_width;
    int32_t letter_height;
    texture_t* font_texture;
} font_t;



enum PAGES {
    PAGE_IN_GAME
};


int32_t page;

extern fbo_t* outport_fbo;

extern texture_t* floor_texture;
extern texture_t* global_texture;

extern font_t letters_font;

extern shader_t* global_shader;
extern shader_t* ui_shader;
extern shader_t* font_shader;
extern shader_t* cooldown_billboards_shader;

extern vec3_t camera_pos;
extern camera_t* camera;
extern camera_t* ui_camera;

extern mesh_t* rect_plane_mesh;
extern mesh_t* cube_mesh;
extern mesh_t* centered_cube_mesh;

extern tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

extern game_t game_struct;
extern ivec3_t selected_tile;
extern ivec2_t hovered_tiles[2];

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;
extern shader_t* sun_shadow_map_shader;




void init_game();

void mouse_press_in_game();

void update_game();
void render_game();

void player_translations_update();
void tile_cooldowns_update();
void update_hovered_tile();
void camera_update();
void sun_shadow_map_update();

void render_game_world();
void render_game_ui();
// returns the drawn width
float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g);

uvec2_t get_str_boxed_size(char* str, float row_height);
void draw_str_boxed(char* str, uint32_t left_x, uint32_t bottom_y, uint32_t padding, uint32_t row_height);

vec2_t outport_space_position_from_world_space(vec3_t pos);
vec2_t get_mouse_outport_space_position();
vec2_t get_mouse_camera_space_position();
vec3_t get_mouse_world_space_position_at_y(float at_y);
ivec2_t get_hovered_tile_position(uint8_t player_i);

void switch_turn();
void player_1_turn();
void player_1_ai_turn();

#endif