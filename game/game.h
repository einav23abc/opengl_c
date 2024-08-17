#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../engine/engine.h"



#define _OUTPORT_WIDTH_ (340*2)
#define _OUTPORT_HEIGHT_ (260*2)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


#define _PLAYER_GRID_WIDTH_ (5)
#define _PLAYER_GRID_DEPTH_ (5)
#define _TILE_SIZE_ (16)
#define _PLAYER_CONSTANT_Z_TRANSLATION_ (-_PLAYER_GRID_DEPTH_*_TILE_SIZE_*0.5)
#define _PLAYER_TRANSLATION_LERP_DURATION_ (500)
#define _WIN_WHEIGHT_ (6)
#define _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_ (2)
#define _SCALE_AXIS_POINT_Y_ (_TILE_SIZE_*(_WIN_WHEIGHT_*0.5))
#define _SCALE_AXIS_LENGTH_ (sqrt(_SCALE_AXIS_POINT_Y_*_SCALE_AXIS_POINT_Y_ + _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_TILE_SIZE_*_TILE_SIZE_))

#define _MAX_TEXT_ROW_LENGTH (64)

#define _MAX_UI_LISTS_AMOUNT_ (4)
#define _UI_LIST_MAX_BUTTONS_AMOUNT_ (8)
#define _UI_LIST_PADDING_ (3)
#define _UI_LIST_BUTTON_HEIGHT_ (12)
#define _UI_LIST_BUTTON_INFO_ROW_HEIGHT_ (12)

#define _MAX_ALERTS_AMOUNT_ (8)
#define _ALERT_PADDING_ (3)
#define _ALERT_ROW_HEIGHT_ (12)


enum TILE_TYPES {
    TILE_TYPE_EMPTY = 0,
    TILE_TYPE_HOUSE,
    TILE_TYPE_BARRACKS,
    TILE_TYPE_FIELD,
    TILE_TYPE_MINE,
    TILE_TYPE_FOREST
};

typedef struct {
    int32_t type;
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
    int32_t wood;
    int32_t stone;
    int32_t wheat;
    int32_t soldiers;
    int32_t population;
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

typedef void(*button_callback_t)(int32_t);

typedef struct {
    uint8_t active : 1;

    uint8_t box_pos_from_world_pos : 1;
    float box_world_pos_x;
    float box_world_pos_y;
    float box_world_pos_z;
    // if box_pos_from_world_pos = 1, this is a screen-x translation
    int32_t x;
    // if box_pos_from_world_pos = 1, this is a screen-y translation
    int32_t y;

    uint32_t buttons_amount;
    char* button_strings[_UI_LIST_MAX_BUTTONS_AMOUNT_];
    char* button_info_strings[_UI_LIST_MAX_BUTTONS_AMOUNT_];
    button_callback_t button_callbacks[_UI_LIST_MAX_BUTTONS_AMOUNT_];

    uint8_t safe : 1;
    int32_t parent_ui_list;
    int32_t child_ui_list;
} ui_list_t;


typedef struct {
    int32_t time_to_live;

    int32_t initial_time_to_live;
    float y_full_transform;
    easing_function_t easing_function;

    uint8_t box_pos_from_world_pos : 1;
    float box_world_pos_x;
    float box_world_pos_y;
    float box_world_pos_z;
    // if box_pos_from_world_pos = 1, this is a screen-x translation
    int32_t x;
    // if box_pos_from_world_pos = 1, this is a screen-y translation
    int32_t y;

    char* string;
} alert_t;


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

extern vec3_t camera_pos;
extern camera_t* camera;
extern camera_t* ui_camera;

extern mesh_t* rect_plane_mesh;
extern mesh_t* cube_mesh;
extern mesh_t* centered_cube_mesh;

extern game_t game_struct;
extern ivec2_t selected_tile;
extern ivec2_t hovered_tiles[2];

extern ui_list_t ui_lists[_MAX_UI_LISTS_AMOUNT_];
extern alert_t alerts[_MAX_ALERTS_AMOUNT_];

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
void update_hovered_tile();
void camera_update();
void sun_shadow_map_update();

void render_game_world();
void render_game_ui();
// returns the drawn width
float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g);

int32_t new_ui_list_assign_id();
void set_ui_lists_to_unsafe();
void close_ui_list(int32_t i);
void make_ui_list_safe(int32_t i);
void close_unsafe_ui_lists();
void close_all_ui_lists();
uint32_t get_ui_list_width(int32_t i);
uint32_t get_ui_list_height(int32_t i);
uvec2_t get_ui_list_box_pos(int32_t i);
uvec2_t get_ui_list_box_pos_padded(int32_t i);
/* x = x pos inside box
 * y = y pos inside box
 * z = ui_list-index
 * or `-1` in everything if not inside ui_list
 */
ivec3_t get_ui_list_inside_pos();

uvec2_t get_str_size(char* str, float row_height);

uvec2_t get_ui_button_info_size(char* info_str);

int32_t new_alert_assign_id();
uvec2_t get_alert_box_size(int32_t i);
uvec2_t get_alert_box_pos(int32_t i);
uvec2_t get_alert_box_pos_padded(int32_t i);
void add_alert_at_cursor(char* string);
void close_all_alerts();

vec2_t outport_space_position_from_world_space(vec3_t pos);
vec2_t get_mouse_outport_space_position();
vec2_t get_mouse_camera_space_position();
vec3_t get_mouse_world_space_position_at_y(float at_y);
ivec2_t get_hovered_tile_position(uint8_t player_i);

void switch_turn();
void player_1_turn();
void player_1_ai_turn();

#endif