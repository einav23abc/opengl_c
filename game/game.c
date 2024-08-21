#include "game.h"


// variable declarations

int32_t page;

fbo_t* outport_fbo;

nine_slice_t nine_slice1;
nine_slice_t nine_slice2;
nine_slice_t nine_slice3;

font_t letters_font;
font_t big_letters_font;

vec3_t camera_pos;
camera_t* camera;
camera_t* ui_camera;

tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

ai_action_func_t ai_build_functions[_AI_BUILD_FUNCTIONS_AMOUNT_] = {
    &player_ai_build_first_least_func,
    &player_ai_build_least_func
};
ai_action_func_t ai_attack_functions[_AI_ATTACK_FUNCTIONS_AMOUNT_] = {
    &player_ai_attack_least_func,
    &player_ai_attack_most_func
};

game_t game_struct;
ai_t current_ai;
ivec3_t selected_tile;
ivec2_t hovered_tiles[2];
int8_t in_cooldowns_translation;
int8_t in_tiles_translation;
int32_t ai_action_cooldown;
int8_t player1_ai_played;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;



vec2_t outport_space_position_from_world_space(vec3_t pos) {
    // for `camera`
    vec2_t osp_pos = vec2_from_vec4(vec4_mul_by_mat4(vec4(pos.x,pos.y,pos.z,1.0), camera->world_view_projection_matrix));
    return vec2_mul(
        vec2_add(vec2_mul_by_scalar(osp_pos, 0.5),vec2(0.5,0.5)),
        vec2(
            _OUTPORT_WIDTH_,
            _OUTPORT_HEIGHT_
        )
    );
}
vec2_t get_mouse_outport_space_position() {
    uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
        w = _OUTPORT_WIDTH_*fpixel_scale;
        h = _OUTPORT_HEIGHT_*fpixel_scale;
    }

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*_OUTPORT_WIDTH_ /w + _OUTPORT_WIDTH_*0.5,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*_OUTPORT_HEIGHT_/h + _OUTPORT_HEIGHT_*0.5
    };
}
vec2_t get_mouse_camera_space_position() {
    uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
        w = _OUTPORT_WIDTH_*fpixel_scale;
        h = _OUTPORT_HEIGHT_*fpixel_scale;
    }

    return (vec2_t){
        .x =  ((float)(mouse.x - 0.5*window_drawable_width ))*camera->width /w,
        .y = -((float)(mouse.y - 0.5*window_drawable_height))*camera->height/h
    };
}
vec3_t get_mouse_world_space_position_at_y(float at_y) {
    // assuming using `camera`-ortho and `outport_fbo`
    // very unoptimised, quickly hacked together

    vec3_t mouse_camera_space_position = vec3_from_vec2(get_mouse_camera_space_position());

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
ivec2_t get_hovered_tile_position(uint8_t player_i) {
    vec3_t world_space_position = get_mouse_world_space_position_at_y(game_struct.players[player_i].y_current_translation);

    return (ivec2_t){
        .x = (int32_t)floor((world_space_position.x - game_struct.players[player_i].x_current_translation)/_TILE_SIZE_),
        .y = (int32_t)floor((world_space_position.z - _PLAYER_CONSTANT_Z_TRANSLATION_)                    /_TILE_SIZE_)
    };
}


int32_t has_enough_resources(int32_t player_id, int32_t tile_type_id) {
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);

    if (
        game_struct.players[player_id].resources.wood       < tile_type->cost.wood       ||
        game_struct.players[player_id].resources.stone      < tile_type->cost.stone      ||
        game_struct.players[player_id].resources.wheat      < tile_type->cost.wheat      ||
        game_struct.players[player_id].resources.population < tile_type->cost.population ||
        game_struct.players[player_id].resources.soldiers   < tile_type->cost.soldiers
    ) {
        return 0;
    }
    return 1;
}

void enter_main_menu() {
    close_all_ui_lists();
    close_all_alerts();
    page = PAGE_MAIN_MENU;
    int32_t ui_list_id;

    // set up ui lists
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 4,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("Play!")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 + big_letters_font.letter_height,

        .buttons_amount = 1,
        .button_strings = {"Play!"},
        .button_info_strings = {"Play against varying enemy AIs"},
        .button_callbacks = {&ui_list_play_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    
    // set up ui lists
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 4,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_*0.5  - (strlen("How to play")*big_letters_font.letter_width + 4*2)*0.5,
        .y = _OUTPORT_HEIGHT_*0.5 + big_letters_font.letter_height*0.5 + 4 - big_letters_font.letter_height,

        .buttons_amount = 1,
        .button_strings = {"How to play"},
        .button_info_strings = {""},
        .button_callbacks = {&ui_list_how_to_play_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}
void enter_how_to_play() {
    close_all_ui_lists();
    close_all_alerts();
    page = PAGE_HOW_TO_PLAY;
    int32_t ui_list_id;

    // exit game - ui list
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - (strlen("Main menu")*letters_font.letter_width + 5*2) - 1,
        .y = _OUTPORT_HEIGHT_ - 1,

        .buttons_amount = 1,
        .button_strings = {"Main menu"},
        .button_info_strings = {""},
        .button_callbacks = {&ui_list_exit_game_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
}
void enter_game() {
    close_all_ui_lists();
    close_all_alerts();
    page = PAGE_IN_GAME;

    // camera
    camera_pos = (vec3_t){
        .x = 0,
        .y = _SCALE_AXIS_POINT_Y_,
        .z = 0
    };
    camera->rx = M_PI*1.8;
    camera->ry = -M_PI*0.15;
    camera->rz = 0;

    selected_tile.x = -1;
    selected_tile.y = -1;
    selected_tile.z = -1;
    hovered_tiles[0].x = -1;
    hovered_tiles[0].y = -1;
    hovered_tiles[1].x = -1;
    hovered_tiles[1].y = -1;

    game_struct.player_turn = 0;
    game_struct.game_ended = 0;


    int32_t ai_build_func_i  = rand() % _AI_BUILD_FUNCTIONS_AMOUNT_;
    int32_t ai_attack_func_i = rand() % _AI_ATTACK_FUNCTIONS_AMOUNT_;
    current_ai = (ai_t) {
        .build_func = ai_build_functions[ai_build_func_i],
        .tile_build_priority  = rand() % _TILE_TYPES_AMOUNT_,
        .tile_build_priority_strength = rand() % 6,
        .attack_func = ai_attack_functions[ai_attack_func_i],
        .tile_attack_priority = rand() % _TILE_TYPES_AMOUNT_
    };
    #ifdef DEBUG_SOFT_MODE
    printf(
        ""      "playing against ai:\n"
        "\t"        "build_func: %d\n"
        "\t"        "build_priority: %d\n"
        "\t"        "build_priority_strength: %d\n"
        "\t"        "\n"
        "\t"        "attack_func: %d\n"
        "\t"        "attack_priority: %d\n"
        ""      "}\n"
        ,
        ai_build_func_i,
        current_ai.tile_build_priority,
        current_ai.tile_build_priority_strength,
        ai_attack_func_i,
        current_ai.tile_attack_priority
    );
    #endif

    ai_action_cooldown = 0;

    in_cooldowns_translation = 0;

    game_struct.players[0].wheight = 0;
    game_struct.players[1].wheight = 0;
    player_translations_update();
    game_struct.players[0].y_lerp_start_translation = game_struct.players[0].y_translation;
    game_struct.players[0].y_current_translation = game_struct.players[0].y_translation;
    game_struct.players[1].y_lerp_start_translation = game_struct.players[1].y_translation;
    game_struct.players[1].y_current_translation = game_struct.players[1].y_translation;

    game_struct.players[0].translation_lerp_time = 0;
    game_struct.players[1].translation_lerp_time = 0;

    for (uint32_t i = 0; i < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; i++) {
        game_struct.players[0].tiles[i].type = TILE_TYPE_EMPTY;
        game_struct.players[0].tiles[i].attacked_effect_time_to_live = 0;
        game_struct.players[0].tiles[i].destroyed_effect_time_to_live = 0;
        game_struct.players[0].tiles[i].built_effect_time_to_live = 0;
        game_struct.players[0].tiles[i].cooldown_timer = 0;
        game_struct.players[0].tiles[i].curent_cooldown_timer = 0;
        game_struct.players[1].tiles[i].type = TILE_TYPE_EMPTY;
        game_struct.players[1].tiles[i].attacked_effect_time_to_live = 0;
        game_struct.players[1].tiles[i].destroyed_effect_time_to_live = 0;
        game_struct.players[1].tiles[i].built_effect_time_to_live = 0;
        game_struct.players[1].tiles[i].cooldown_timer = 0;
        game_struct.players[1].tiles[i].curent_cooldown_timer = 0;
    }
    int32_t rand_pos;
    rand_pos = get_random_empty_tile_position(0);
    game_struct.players[0].tiles[rand_pos].type = TILE_TYPE_HOUSE;
    game_struct.players[0].tiles[rand_pos].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[0].tiles[rand_pos].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    rand_pos = get_random_empty_tile_position(0);
    game_struct.players[0].tiles[rand_pos].type = TILE_TYPE_FOREST;
    game_struct.players[0].tiles[rand_pos].cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    game_struct.players[0].tiles[rand_pos].curent_cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    rand_pos = get_random_empty_tile_position(1);
    game_struct.players[1].tiles[rand_pos].type = TILE_TYPE_HOUSE;
    game_struct.players[1].tiles[rand_pos].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[1].tiles[rand_pos].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    rand_pos = get_random_empty_tile_position(1);
    game_struct.players[1].tiles[rand_pos].type = TILE_TYPE_FOREST;
    game_struct.players[1].tiles[rand_pos].cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    game_struct.players[1].tiles[rand_pos].curent_cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;

    game_struct.players[0].resources.wood = 2;
    game_struct.players[0].resources.stone = 2;
    game_struct.players[0].resources.wheat = 2;
    game_struct.players[0].resources.soldiers = 0;
    game_struct.players[0].resources.population = 2;
    game_struct.players[1].resources.wood = 2;
    game_struct.players[1].resources.stone = 2;
    game_struct.players[1].resources.wheat = 2;
    game_struct.players[1].resources.soldiers = 0;
    game_struct.players[1].resources.population = 2;

    // next turn - ui list
    int32_t ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 7,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = 1,
        .y = _OUTPORT_HEIGHT_ - 1,

        .buttons_amount = 1,
        .button_strings = {"End turn"},
        .button_info_strings = {""},
        .button_callbacks = {&switch_turn_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    
    // exit game - ui list
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - (strlen("Exit game")*letters_font.letter_width + 5*2) - 1,
        .y = _OUTPORT_HEIGHT_ - 1,

        .buttons_amount = 1,
        .button_strings = {"Exit game"},
        .button_info_strings = {""},
        .button_callbacks = {&ui_list_exit_in_game_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };



    // type infos - ui list hack
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 1,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - ((2)*big_letters_font.letter_width + 6*2) + 1,
        .y = (big_letters_font.letter_height + 6*2) - 1,

        .buttons_amount = 1,
        .button_strings = {"\x19\x1a"},
        .button_info_strings = {"Soldiers \x19\x1a:\n* Created by the Barracks.\n* Use on enemy tiles to destroy buildings."},
        .button_callbacks = {NULL},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 1,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - ((2+4)*big_letters_font.letter_width + 6*2) + 1,
        .y = (big_letters_font.letter_height + 6*2) - 1,

        .buttons_amount = 1,
        .button_strings = {"\x17\x18"},
        .button_info_strings = {"Stone \x17\x18:\n* Created by the Mine."},
        .button_callbacks = {NULL},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 1,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - ((2+8)*big_letters_font.letter_width + 6*2) + 1,
        .y = (big_letters_font.letter_height + 6*2) - 1,

        .buttons_amount = 1,
        .button_strings = {"\x15\x16"},
        .button_info_strings = {"Wood \x15\x16:\n* Created by the Forest."},
        .button_callbacks = {NULL},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 1,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - ((2+12)*big_letters_font.letter_width + 6*2) + 1,
        .y = (big_letters_font.letter_height + 6*2) - 1,

        .buttons_amount = 1,
        .button_strings = {"\x13\x14"},
        .button_info_strings = {"Wheat \x13\x14:\n* Created by the Field."},
        .button_callbacks = {NULL},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };
    ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .hidden = 1,

        .font = &big_letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 5,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - ((2+16)*big_letters_font.letter_width + 6*2) + 1,
        .y = (big_letters_font.letter_height + 6*2) - 1,

        .buttons_amount = 1,
        .button_strings = {"\x11\x12"},
        .button_info_strings = {"People \x11\x12:\n* Created by the House."},
        .button_callbacks = {NULL},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };

    page = PAGE_IN_GAME;
}
void switch_turn_button_callback(int32_t ui_list_id, int32_t button_id) {
    request_switch_turn();
}
void request_switch_turn() {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    if (game_struct.player_turn == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("It is not your turn");
        return;
    }
    switch_turn();
}
void switch_turn() {
    if (game_struct.game_ended == 1) return;

    audio_sound_play(switch_turn_sound);

    game_struct.player_turn = !game_struct.player_turn;
    selected_tile.x = -1;
    selected_tile.y = -1;
    close_unperm_ui_lists();

    ai_action_cooldown = 1000;
    player1_ai_played = 0;

    // update cooldown timers
    for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
        for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
            tile_t* tile = &(game_struct.players[game_struct.player_turn].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
            
            if (tile->type == TILE_TYPE_EMPTY) continue;

            tile->cooldown_timer -= 1;

            if (tile->cooldown_timer <= 0) {
                tile->cooldown_timer = tile_type_properties[tile->type].give_cooldown;
            }
        }
    }
}
void build_at_tile(int32_t player, int32_t tile_type_id, int32_t at_tile) {
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);
    
    audio_sound_play(build_tile_sound);
    if (player == 0) {
        audio_sound_play(wheight_up_sound);
    }else {
        audio_sound_play(wheight_down_sound);
    }

    game_struct.players[player].wheight += 1;
    game_struct.players[1-player].wheight -= 1;

    game_struct.players[player].resources.wood       -= tile_type->cost.wood;
    game_struct.players[player].resources.stone      -= tile_type->cost.stone;
    game_struct.players[player].resources.wheat      -= tile_type->cost.wheat;
    game_struct.players[player].resources.population -= tile_type->cost.population;
    game_struct.players[player].resources.soldiers   -= tile_type->cost.soldiers;
    game_struct.players[player].tiles[at_tile].cooldown_timer        = tile_type->give_cooldown;
    game_struct.players[player].tiles[at_tile].curent_cooldown_timer = tile_type->give_cooldown;
    game_struct.players[player].tiles[at_tile].built_effect_time_to_live = _TILE_BUILT_EFFECT_TIME_;
    game_struct.players[player].tiles[at_tile].type = tile_type_id;
}
void attack_tile(int32_t player_attacked, int32_t at_tile) {
    audio_sound_play(attack_tile_sound);
    if (player_attacked == 1) {
        audio_sound_play(wheight_up_sound);
    }else {
        audio_sound_play(wheight_down_sound);
    }

    game_struct.players[1-player_attacked].resources.soldiers -= 1;
    game_struct.players[1-player_attacked].wheight += 1;
    game_struct.players[player_attacked].wheight -= 1;

    game_struct.players[player_attacked].tiles[at_tile].type = TILE_TYPE_EMPTY;
    game_struct.players[player_attacked].tiles[at_tile].cooldown_timer        = 0;
    game_struct.players[player_attacked].tiles[at_tile].curent_cooldown_timer = 0;
    game_struct.players[player_attacked].tiles[at_tile].attacked_effect_time_to_live = _TILE_ATTACKED_EFFECT_TIME_;
    game_struct.players[player_attacked].tiles[at_tile].destroyed_effect_time_to_live = _TILE_DESTROYED_EFFECT_TIME_;
}
tile_types_amount_sorted_t get_tile_types_amounts_sorted(int32_t player) {
    tile_types_amount_sorted_t ttas;

    // count amounts
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) ttas.tile_types_amounts[i] = 0;
    for (uint32_t x = 0; x < _PLAYER_GRID_WIDTH_; x++) {
        for (uint32_t z = 0; z < _PLAYER_GRID_DEPTH_; z++) {
            tile_t* tile = &(game_struct.players[player].tiles[z*_PLAYER_GRID_DEPTH_ + x]);
            if (tile->type == TILE_TYPE_EMPTY) continue;
            ttas.tile_types_amounts[tile->type] += 1;
        }
    }

    // sort amounts
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) ttas.sorted_tile_type_id[i] = i;
    // bubble sort
    for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_-1; i++) {
        for (uint32_t j = 0; j < _TILE_TYPES_AMOUNT_-1-i; j++) {
            if (
                (ttas.tile_types_amounts[ttas.sorted_tile_type_id[j]] > ttas.tile_types_amounts[ttas.sorted_tile_type_id[j+1]]) ||
                // put some randomness into equal amount tiles
                (
                    ttas.tile_types_amounts[ttas.sorted_tile_type_id[j]] == ttas.tile_types_amounts[ttas.sorted_tile_type_id[j+1]] &&
                    rand()%2 == 1
                )
            ) {
                int32_t tmp = ttas.sorted_tile_type_id[j];
                ttas.sorted_tile_type_id[j  ] = ttas.sorted_tile_type_id[j+1];
                ttas.sorted_tile_type_id[j+1] = tmp;
            }
        }
    }

    return ttas;
}
int32_t get_random_empty_tile_position(int32_t player) {
    // check if empty space exists
    int32_t empty_tile_exists = 0;
    for (uint32_t j = 0; j < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; j++) {
        if (game_struct.players[player].tiles[j].type == TILE_TYPE_EMPTY) {
            empty_tile_exists = 1;
            break;
        }
    }
    if (empty_tile_exists == 0) return -1;

    while (1) {
        int32_t tile_pos = rand() % (_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_);
        if (game_struct.players[player].tiles[tile_pos].type == TILE_TYPE_EMPTY) return tile_pos;
    }
}
void player_1_turn() {
    player_1_ai_turn();
}
void player_1_ai_turn() {
    // dont resolve turn while transitions (animations) or cooldowns are playing
    if (ai_action_cooldown > 0) {
        ai_action_cooldown -= delta_time;
        return;
    }
    if (in_cooldowns_translation == 1) return;
    if (in_tiles_translation == 1) return;

    if (player1_ai_played == 1) {
        switch_turn();
        return;
    }

    int32_t built = current_ai.build_func();
    if (built) {
        // dont do actions too fast
        ai_action_cooldown = 1000;
        return;
    }

    int32_t attacked = current_ai.attack_func();
    if (attacked) {
        // dont do actions too fast
        ai_action_cooldown = 1000;
        return;
    }

    // end turn after transitions will end
    player1_ai_played = 1;
}
// builds 'priority' orthe first thing it can (in order of what it has least of)
int32_t player_ai_build_first_least_func() {
    tile_types_amount_sorted_t ttas = get_tile_types_amounts_sorted(game_struct.player_turn);

    // build whatever i can
    for (int32_t i = -1; i < _TILE_TYPES_AMOUNT_-1; i++) {
        int32_t tile_type;

        if (i == -1) {
            // build priority
            tile_type = current_ai.tile_build_priority;
            // build sometimes according to strength
            if (rand() % (current_ai.tile_build_priority_strength+1) != 0) continue;
        }else {
            // build sorted
            tile_type = ttas.sorted_tile_type_id[i];
        }

        // dont build if not enough resources
        if (has_enough_resources(1, tile_type) == 0) continue;

        // find space to build
        int32_t new_tile_grid_index = get_random_empty_tile_position(game_struct.player_turn);
        // handle no space
        if (new_tile_grid_index == -1) break;

        build_at_tile(game_struct.player_turn, tile_type, new_tile_grid_index);

        // built something
        return 1;
    }

    // didnt build anything
    return 0;
}
// builds 'priority' or what it has least of, and only things with this 'least amount'
int32_t player_ai_build_least_func() {
    tile_types_amount_sorted_t ttas = get_tile_types_amounts_sorted(game_struct.player_turn);

    int32_t least_amount = ttas.tile_types_amounts[ttas.sorted_tile_type_id[0]];

    // build only things that have 'least amount'
    for (int32_t i = -1; i < _TILE_TYPES_AMOUNT_-1; i++) {
        int32_t tile_type;

        if (i == -1) {
            // build priority
            tile_type = current_ai.tile_build_priority;
            // build sometimes according to strength
            if (rand() % (current_ai.tile_build_priority_strength+1) != 0) continue;
        }else {
            // build sorted
            tile_type = ttas.sorted_tile_type_id[i];
            // dont build things that there are plenty of
            if (ttas.tile_types_amounts[tile_type] > least_amount) break;
        }

        // dont build if not enough resources
        if (has_enough_resources(1, tile_type) == 0) continue;

        // find space to build
        int32_t new_tile_grid_index = get_random_empty_tile_position(game_struct.player_turn);
        // handle no space
        if (new_tile_grid_index == -1) break;

        build_at_tile(game_struct.player_turn, tile_type, new_tile_grid_index);

        // built something
        return 1;
    }

    // didnt build anything
    return 0;
}
// attacks 'priority' or whatever the other player has least of
int32_t player_ai_attack_least_func() {
    tile_types_amount_sorted_t ttas = get_tile_types_amounts_sorted(1-game_struct.player_turn);

    // cant attack
    if (game_struct.players[game_struct.player_turn].resources.soldiers < 1) return 0;

    // attack the building that has the least amount of apperances
    for (int32_t i = -1; i < _TILE_TYPES_AMOUNT_-1; i++) {
        int32_t tile_type;
        if (i == -1) {
            tile_type = current_ai.tile_attack_priority;
            if (tile_type < 0 || tile_type >= _TILE_TYPES_AMOUNT_ || tile_type == TILE_TYPE_EMPTY) continue;
        }else {
            // least amount
            tile_type = ttas.sorted_tile_type_id[i];
        }

        // no apperances
        if (ttas.tile_types_amounts[tile_type] <= 0) continue;

        // find first apperance
        for (uint32_t j = 0; j < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; j++) {
            if (game_struct.players[1-game_struct.player_turn].tiles[j].type == tile_type) {
                attack_tile(1-game_struct.player_turn, j);
                return 1;
            }
        }
    }

    // didnt attack anything - must be empty?
    return 0;
}
// attacks 'priority' or whatever the other player has most of
int32_t player_ai_attack_most_func() {
    tile_types_amount_sorted_t ttas = get_tile_types_amounts_sorted(1-game_struct.player_turn);

    // cant attack
    if (game_struct.players[game_struct.player_turn].resources.soldiers < 1) return 0;

    // attack the building that has the least amount of apperances
    for (int32_t i = -1; i < _TILE_TYPES_AMOUNT_-1; i++) {
        int32_t tile_type;
        if (i == -1) {
            tile_type = current_ai.tile_attack_priority;
            if (tile_type < 0 || tile_type >= _TILE_TYPES_AMOUNT_ || tile_type == TILE_TYPE_EMPTY) continue;
        }else {
            // most amount
            tile_type = ttas.sorted_tile_type_id[_TILE_TYPES_AMOUNT_-1-i];
        }

        // no apperances
        if (ttas.tile_types_amounts[tile_type] <= 0) continue;

        // find first apperance
        for (uint32_t j = 0; j < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; j++) {
            if (game_struct.players[1-game_struct.player_turn].tiles[j].type == tile_type) {
                attack_tile(1-game_struct.player_turn, j);
                return 1;
            }
        }
    }

    // didnt attack anything - must be empty?
    return 0;
}


void ui_list_how_to_play_button_callback(int32_t ui_list_id, int32_t button_id) {
    enter_how_to_play();
}
void ui_list_exit_in_game_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        ui_list_exit_game_button_callback(-1,-1);
        return;
    }

    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 0,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 2,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 0,
        .x = ui_lists[ui_list_id].x + get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding - (strlen("Are you sure?")*letters_font.letter_width + 2*2 + 1),
        .y = ui_lists[ui_list_id].y - get_ui_list_height(ui_list_id) - ui_lists[ui_list_id].padding,

        .buttons_amount = 2,
        .button_strings = {"Yes", "Are you sure?"},
        .button_info_strings = {"", ""},
        .button_callbacks = {
            &ui_list_exit_game_button_callback,
            NULL
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}
void ui_list_exit_game_button_callback(int32_t ui_list_id, int32_t button_id) {
    enter_main_menu();
}
void ui_list_play_button_callback(int32_t ui_list_id, int32_t button_id) {
    enter_game();
}
void ui_list_build_specific_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    int32_t tile_type_id = button_id;

    if (has_enough_resources(0, tile_type_id) == 0) {
        // not enough resources
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }
    
    build_at_tile(0, tile_type_id, selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x);

    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void ui_list_build_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 0,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 2,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = ui_lists[ui_list_id].box_world_pos_x,
        .box_world_pos_y = ui_lists[ui_list_id].box_world_pos_y,
        .box_world_pos_z = ui_lists[ui_list_id].box_world_pos_z,
        .x = get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding,
        .y = 0,

        .buttons_amount = _TILE_TYPES_AMOUNT_,
        .button_strings = {"House", "Barracks", "Field", "Mine", "Forest"},
        .button_info_strings = {
            tile_type_properties[0].build_info_string,
            tile_type_properties[1].build_info_string,
            tile_type_properties[2].build_info_string,
            tile_type_properties[3].build_info_string,
            tile_type_properties[4].build_info_string
        },
        .button_callbacks = {
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback,
            &ui_list_build_specific_button_callback
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}
void ui_list_attack_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    if (game_struct.players[0].resources.soldiers < 1) {
        // not enough resources
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }
    
    attack_tile(1, selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x);
    
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void ui_list_demolish_sure_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }
    
    audio_sound_play(demolish_tile_sound);
    audio_sound_play(wheight_down_sound);

    game_struct.players[0].wheight -= 1;
    game_struct.players[1].wheight += 1;

    int32_t tile_type_id = game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type;
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);
    
    // add resources
    game_struct.players[0].resources.wood       += tile_type->give.wood;
    game_struct.players[0].resources.stone      += tile_type->give.stone;
    game_struct.players[0].resources.wheat      += tile_type->give.wheat;
    game_struct.players[0].resources.population += tile_type->give.population;
    game_struct.players[0].resources.soldiers   += tile_type->give.soldiers;
    // add alert
    int32_t new_alert_id = new_alert_assign_id();
    alerts[new_alert_id] = (alert_t){
        .time_to_live = 3000,

        .font = &letters_font,
        .padding = 3,

        .initial_time_to_live = 3000,
        .y_full_transform = letters_font.letter_height*3,
        .easing_function = &ease_out_sine,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = 0.5*_TILE_SIZE_ + selected_tile.x*_TILE_SIZE_ + game_struct.players[0].x_current_translation,
        .box_world_pos_y =   1*_TILE_SIZE_ + game_struct.players[0].y_current_translation,
        .box_world_pos_z = 0.5*_TILE_SIZE_ + selected_tile.y*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
        .x = 0,
        .y = 0,
        
        .string = tile_type->give_alert_string
    };

    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].type = TILE_TYPE_EMPTY;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].cooldown_timer        = 0;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].curent_cooldown_timer = 0;
    game_struct.players[0].tiles[selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x].destroyed_effect_time_to_live = _TILE_DESTROYED_EFFECT_TIME_;
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void ui_list_demolish_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        audio_sound_play(error_sound);
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    // close child (if exists)
    close_ui_list(ui_lists[ui_list_id].child_ui_list);

    // create child
    int32_t new_ui_list_id = new_ui_list_assign_id();
    ui_lists[new_ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 0,

        .hidden = 0,

        .font = &letters_font,
        .info_string_font = &letters_font,
        .padding = 1,
        .button_padding = 2,
        .box_nslice = &nine_slice2,
        .button_hover_nslice = &nine_slice3,
        .info_string_nslice = &nine_slice1,

        .box_pos_from_world_pos = 1,
        .box_world_pos_x = ui_lists[ui_list_id].box_world_pos_x,
        .box_world_pos_y = ui_lists[ui_list_id].box_world_pos_y,
        .box_world_pos_z = ui_lists[ui_list_id].box_world_pos_z,
        .x = get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding,
        .y = 0,

        .buttons_amount = 2,
        .button_strings = {"Yes", "Are you sure?"},
        .button_info_strings = {"", ""},
        .button_callbacks = {
            &ui_list_demolish_sure_button_callback,
            NULL
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}