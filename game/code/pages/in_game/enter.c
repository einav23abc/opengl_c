#include "page.h"
#include "against_ai.h"
#include "against_connected.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

static void switch_turn_button_callback(int32_t ui_list_id, int32_t button_id);
static void exit_in_game_callback(int32_t ui_list_id, int32_t button_id);


void enter_in_game() {
    close_all_ui_lists();
    close_all_alerts();

    // camera
    camera_pos = (vec3_t){
        .x = 0,
        .y = _SCALE_AXIS_POINT_Y_,
        .z = 0
    };
    camera->rx = M_PI*1.8;
    camera->ry = -M_PI*0.15;
    camera->rz = 0;

    // selected and hovered
    selected_tile.x = -1;
    selected_tile.y = -1;
    selected_tile.z = -1;
    hovered_tiles[0].x = -1;
    hovered_tiles[0].y = -1;
    hovered_tiles[1].x = -1;
    hovered_tiles[1].y = -1;

    // game struct ; if playing in LAN the game struct will be set by `generate_state_packet` or `parse_state_packet` 
    if (play_type == PLAY_TYPE_AGAINST_AI) {
        init_game_struct();
    }

    if (play_type == PLAY_TYPE_AGAINST_AI) {
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
    }

    in_cooldowns_translation = 0;


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
        .button_callbacks = {&exit_in_game_callback},

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


static void switch_turn_button_callback(int32_t ui_list_id, int32_t button_id) {
    request_switch_turn();
}
static void exit_in_game_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        exit_game_button_callback(-1,-1);
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
            &exit_game_button_callback,
            NULL
        },

        .child_ui_list = -1,
        .parent_ui_list = ui_list_id
    };
    ui_lists[ui_list_id].child_ui_list = new_ui_list_id;

    make_ui_list_safe(new_ui_list_id);
}