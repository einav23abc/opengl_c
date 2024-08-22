#ifndef PAGE_IN_GAME_H
#define PAGE_IN_GAME_H

#include "../../game.h"

#define _PLAYER_GRID_WIDTH_ (4)
#define _PLAYER_GRID_DEPTH_ (4)
#define _TILE_SIZE_ (20)
#define _PLAYER_CONSTANT_Z_TRANSLATION_ (-_PLAYER_GRID_DEPTH_*_TILE_SIZE_*0.5)
#define _PLAYER_TRANSLATION_LERP_DURATION_ (500)
#define _PLAYER_COOLDOWN_TRANSLATION_LERP_DURATION_ (500)
#define _WIN_WHEIGHT_ (5)
#define _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_ (2)
#define _SCALE_AXIS_POINT_Y_ (_TILE_SIZE_*(_WIN_WHEIGHT_*0.5))
#define _SCALE_AXIS_LENGTH_ (sqrt(_SCALE_AXIS_POINT_Y_*_SCALE_AXIS_POINT_Y_ + _HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_HINGE_DISTANCE_FROM_SCALE_AT_BOTTOM_*_TILE_SIZE_*_TILE_SIZE_))

#define _TILE_BUILD_INFO_STRING_MAX_LENGTH_ (64)
#define _TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_ (64)
#define _TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_ (64)
// excluding TILE_TYPE_EMPTY
#define _TILE_TYPES_AMOUNT_ (5)
#define _TILE_ATTACKED_EFFECT_TIME_ (1500)
#define _TILE_DESTROYED_EFFECT_TIME_ (1000)
#define _TILE_BUILT_EFFECT_TIME_ (1000)

#define _AI_BUILD_FUNCTIONS_AMOUNT_ (2)
#define _AI_ATTACK_FUNCTIONS_AMOUNT_ (2)

#define _CAMERA_MOVE_SPEED_ (0.5)


typedef struct {
    int32_t wood;
    int32_t stone;
    int32_t wheat;
    int32_t population;
    int32_t soldiers;
} resources_t;

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

typedef struct {
    int32_t type;
    float curent_cooldown_timer;
    int32_t cooldown_timer;
    int32_t attacked_effect_time_to_live;
    int32_t destroyed_effect_time_to_live;
    int32_t built_effect_time_to_live;
    // uint8_t camoflauged : 1;
} tile_t;

typedef struct {
    int32_t tile_types_amounts[_TILE_TYPES_AMOUNT_];
    int32_t sorted_tile_type_id[_TILE_TYPES_AMOUNT_];
} tile_types_amount_sorted_t;

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

typedef int32_t(*ai_action_func_t)(void);

typedef struct {
    int32_t tile_build_priority;
    int32_t tile_build_priority_strength; // 0 = always, 1 = 1/2 the time, 2 = 1/3 of the time
    ai_action_func_t build_func;
    int32_t tile_attack_priority;
    ai_action_func_t attack_func;
} ai_t;


extern tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

extern ai_action_func_t ai_build_functions[_AI_BUILD_FUNCTIONS_AMOUNT_];
extern ai_action_func_t ai_attack_functions[_AI_ATTACK_FUNCTIONS_AMOUNT_];

extern game_t game_struct;
extern ai_t current_ai;
extern ivec3_t selected_tile;
extern ivec2_t hovered_tiles[2];
extern int8_t in_cooldowns_translation;
extern int8_t in_tiles_translation;
extern int32_t ai_action_cooldown;
extern int8_t player1_ai_played;

extern vec3_t camera_pos;
extern camera_t* camera;

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;



// <page functions>
void init_in_game();
void enter_in_game();
void update_in_game();
void render_in_game();
void mouse_press_in_game();
// </page functions>

void player_translations_update();

ivec2_t get_hovered_tile_position(uint8_t player_i);

int32_t has_enough_resources(int32_t player_id, int32_t tile_type_id);

void request_switch_turn();
void switch_turn();
int32_t build_at_tile(int32_t player, int32_t tile_type_id, int32_t at_tile);
void attack_tile(int32_t player_attacked, int32_t at_tile);

tile_types_amount_sorted_t get_tile_types_amounts_sorted(int32_t player);
int32_t get_random_empty_tile_position(int32_t player);
void player_1_turn();
void player_1_ai_turn();
int32_t player_ai_build_first_least_func();
int32_t player_ai_build_least_func();
int32_t player_ai_attack_least_func();
int32_t player_ai_attack_most_func();

void exit_game_button_callback(int32_t ui_list_id, int32_t button_id);
void build_specific_button_callback(int32_t ui_list_id, int32_t button_id);
void build_button_callback(int32_t ui_list_id, int32_t button_id);
void attack_button_callback(int32_t ui_list_id, int32_t button_id);
void demolish_sure_button_callback(int32_t ui_list_id, int32_t button_id);
void demolish_button_callback(int32_t ui_list_id, int32_t button_id);

#endif