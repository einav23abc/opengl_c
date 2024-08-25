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
        int32_t starting_tile_positions[4];
        generate_starting_tile_positions(starting_tile_positions);
        game_struct_set_starting_tiles(starting_tile_positions);
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


    int32_t ui_list_id;

    // next turn - ui list
    ui_list_id = new_ui_list(
        1,
        _OUTPORT_HEIGHT_ - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &switch_turn_button_callback,
                .string = "End turn",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    
    // exit game - ui list
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - (strlen("Exit game")*letters_font.letter_width + 5*2) - 1,
        _OUTPORT_HEIGHT_ - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &exit_in_game_callback,
                .string = "Exit game",
                .font = &letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );

    // type infos - ui list hack
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - ((2)*big_letters_font.letter_width + 6*2) + 1,
        (big_letters_font.letter_height + 6*2) - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    make_ui_list_hidden(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "\x19\x1a",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Soldiers \x19\x1a:\n* Created by the Barracks.\n* Use on enemy tiles to destroy buildings.",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - ((2+4)*big_letters_font.letter_width + 6*2) + 1,
        (big_letters_font.letter_height + 6*2) - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    make_ui_list_hidden(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "\x17\x18",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Stone \x17\x18:\n* Created by the Mine.",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - ((2+8)*big_letters_font.letter_width + 6*2) + 1,
        (big_letters_font.letter_height + 6*2) - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    make_ui_list_hidden(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "\x15\x16",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Wood \x15\x16:\n* Created by the Forest.",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );

    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - ((2+12)*big_letters_font.letter_width + 6*2) + 1,
        (big_letters_font.letter_height + 6*2) - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    make_ui_list_hidden(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "\x13\x14",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "Wheat \x13\x14:\n* Created by the Field.",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );

    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - ((2+16)*big_letters_font.letter_width + 6*2) + 1,
        (big_letters_font.letter_height + 6*2) - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    make_ui_list_hidden(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "\x11\x12",
                .font = &big_letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "People \x11\x12:\n* Created by the House.",
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
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
    int32_t new_ui_list_id = new_ui_list_child(
        ui_list_id,
        get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding - (strlen("Are you sure?")*letters_font.letter_width + 3*2 + 2),
        - get_ui_list_height(ui_list_id) - ui_lists[ui_list_id].padding - 2 + 1,
        2,
        &nine_slice1
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &exit_game_button_callback,
                .string = "Yes",
                .font = &letters_font,
                .padding = 3,
                .nslice = &nine_slice2,
                .hover_nslice = &nine_slice4,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = "Are you sure?",
                .font = &letters_font,
                .padding = 3,
                .nslice = NULL,
                .hover_nslice = NULL,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    make_ui_list_safe(new_ui_list_id);
}