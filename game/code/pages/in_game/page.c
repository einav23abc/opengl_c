#include "page.h"
#include "against_ai.h"
#include "against_connected.h"
#include "../../sounds.h"
#include "../../spaces.h"
#include "../../ui_lists.h"
#include "../../alerts.h"


tile_type_t tile_type_properties[_TILE_TYPES_AMOUNT_];

game_t game_struct;
ivec3_t selected_tile;
ivec2_t hovered_tiles[2];
int8_t in_cooldowns_translation;
int8_t in_tiles_translation;
PLAY_TYPE play_type;

vec3_t camera_pos;
camera_t* camera;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;



void player_translations_update() {
    in_tiles_translation = 0;
    
    for (uint8_t i = 0; i < 2; i++) {
        // set translation
        int32_t showed_wheight = game_struct.players[i].wheight;
        if (showed_wheight > _WIN_WHEIGHT_) {
            showed_wheight = _WIN_WHEIGHT_;
        }else if (showed_wheight < -_WIN_WHEIGHT_) {
            showed_wheight = -_WIN_WHEIGHT_;
        }

        float last_y_translation = game_struct.players[i].y_translation;

        game_struct.players[i].y_translation = (-showed_wheight*0.5)*_TILE_SIZE_+_SCALE_AXIS_POINT_Y_ + _TILE_SIZE_*0.5;
        if (last_y_translation != game_struct.players[i].y_translation) {
            in_tiles_translation = 1;

            game_struct.players[i].y_lerp_start_translation = game_struct.players[i].y_current_translation;
            game_struct.players[i].translation_lerp_time = 0;
        }

        // animate current translation
        if (game_struct.players[i].y_current_translation != game_struct.players[i].y_translation) {
            in_tiles_translation = 1;

            game_struct.players[i].translation_lerp_time = min(game_struct.players[i].translation_lerp_time + delta_time, _PLAYER_TRANSLATION_LERP_DURATION_);
            float t_linear = ((float)(game_struct.players[i].translation_lerp_time))/_PLAYER_TRANSLATION_LERP_DURATION_;
            float t = ease_out_back(t_linear);
            game_struct.players[i].y_current_translation = (1-t)*(game_struct.players[i].y_lerp_start_translation) + (t)*(game_struct.players[i].y_translation);
        }

        // set hinge_y_position
        game_struct.players[i].hinge_y_position = game_struct.players[i].y_current_translation - _TILE_SIZE_*0.5;
        
        // set hinge_x_position according to hinge_y_position
        game_struct.players[i].hinge_x_position = fabs(
            sqrt(
                (_SCALE_AXIS_LENGTH_*_SCALE_AXIS_LENGTH_) -
                ((_SCALE_AXIS_POINT_Y_-game_struct.players[i].hinge_y_position)*(_SCALE_AXIS_POINT_Y_-game_struct.players[i].hinge_y_position))
            )
        );
        if (i == 0) {
            game_struct.players[i].hinge_x_position = -(game_struct.players[i].hinge_x_position);
        }

        // set x_current_translation
        if (i == 0) {
            game_struct.players[i].x_current_translation = game_struct.players[i].hinge_x_position - _TILE_SIZE_ - _TILE_SIZE_*_PLAYER_GRID_WIDTH_;
        }else {
            game_struct.players[i].x_current_translation = game_struct.players[i].hinge_x_position + _TILE_SIZE_;
        }
    }
}

ivec2_t get_hovered_tile_position(uint8_t player_i) {
    vec3_t world_space_position = world_position_at_y_from_mouse_position(outport_fbo, camera, game_struct.players[player_i].y_current_translation);

    return (ivec2_t){
        .x = (int32_t)floor((world_space_position.x - game_struct.players[player_i].x_current_translation)/_TILE_SIZE_),
        .y = (int32_t)floor((world_space_position.z - _PLAYER_CONSTANT_Z_TRANSLATION_)                    /_TILE_SIZE_)
    };
}

void request_switch_turn() {
    if (game_struct.game_ended == 1) {
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    if (game_struct.player_turn == 1) {
        add_error_alert_at_cursor("It is not your turn");
        return;
    }
    switch_turn();
}
void switch_turn() {
    if (game_struct.game_ended == 1) return;

    audio_sound_play(switch_turn_sound);

    if (game_struct.player_turn == 0 && play_type != PLAY_TYPE_AGAINST_AI) {
        nf_packet_t packet = (nf_packet_t){
            .packet_len = 3,
            .packet_type = CLIENT_UPDATE,
            .client_id = my_client_id,
            .packet_body[0] = GAME_UPDATE_END_TURN
        };
        
        if (play_type == PLAY_TYPE_AGAINST_HOST) {
            send_update_packet(packet);
        }
        if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
            send_update_packet_as_server(packet);
        }
    }

    game_struct.player_turn = 1-game_struct.player_turn;
    selected_tile.x = -1;
    selected_tile.y = -1;
    close_unperm_ui_lists();

    if (play_type == PLAY_TYPE_AGAINST_AI) {
        ai_action_cooldown = 1000;
        player1_ai_played = 0;
    }

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
int32_t build_at_tile(int32_t player, int32_t tile_type_id, int32_t at_tile) {
    // dont build if not enough resources
    if (has_enough_resources(player, tile_type_id) == 0) return 0;

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

    if (player == 0 && play_type != PLAY_TYPE_AGAINST_AI) {
        nf_packet_t packet = (nf_packet_t){
            .packet_len = 3,
            .packet_type = CLIENT_UPDATE,
            .client_id = my_client_id,
            .packet_body[0] = GAME_UPDATE_BUILD
        };
        memcpy(&(packet.packet_body[1]),                      &tile_type_id, sizeof(tile_type_id));
        packet.packet_len += sizeof(tile_type_id);
        memcpy(&(packet.packet_body[1+sizeof(tile_type_id)]), &at_tile, sizeof(at_tile));
        packet.packet_len += sizeof(at_tile);

        if (play_type == PLAY_TYPE_AGAINST_HOST) {
            send_update_packet(packet);
        }
        if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
            send_update_packet_as_server(packet);
        }
    }
    return 1;
}
int32_t attack_tile(int32_t player_attacked, int32_t at_tile) {
    if (game_struct.players[1-player_attacked].resources.soldiers < 1) return 0;

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

    if (1-player_attacked == 0 && play_type != PLAY_TYPE_AGAINST_AI) {
        nf_packet_t packet = (nf_packet_t){
            .packet_len = 3,
            .packet_type = CLIENT_UPDATE,
            .client_id = my_client_id,
            .packet_body[0] = GAME_UPDATE_ATTACK
        };
        memcpy(&(packet.packet_body[1]), &at_tile, sizeof(at_tile));
        packet.packet_len += sizeof(at_tile);
        
        if (play_type == PLAY_TYPE_AGAINST_HOST) {
            send_update_packet(packet);
        }
        if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
            send_update_packet_as_server(packet);
        }
    }

    return 1;
}
void demolish_tile(int32_t player, int32_t at_tile) {
    int32_t tile_type_id = game_struct.players[player].tiles[at_tile].type;
    if (tile_type_id == TILE_TYPE_EMPTY) return;
    tile_type_t* tile_type = &(tile_type_properties[tile_type_id]);

    int32_t tile_x = at_tile % _PLAYER_GRID_WIDTH_;
    int32_t tile_z = at_tile / _PLAYER_GRID_WIDTH_;

    audio_sound_play(demolish_tile_sound);
    audio_sound_play(wheight_down_sound);

    game_struct.players[player].wheight -= 1;
    game_struct.players[1-player].wheight += 1;
    
    // add resources
    game_struct.players[player].resources.wood       += tile_type->give.wood;
    game_struct.players[player].resources.stone      += tile_type->give.stone;
    game_struct.players[player].resources.wheat      += tile_type->give.wheat;
    game_struct.players[player].resources.population += tile_type->give.population;
    game_struct.players[player].resources.soldiers   += tile_type->give.soldiers;
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
        .box_world_pos_x = 0.5*_TILE_SIZE_ + tile_x*_TILE_SIZE_ + game_struct.players[player].x_current_translation,
        .box_world_pos_y =   1*_TILE_SIZE_ + game_struct.players[player].y_current_translation,
        .box_world_pos_z = 0.5*_TILE_SIZE_ + tile_z*_TILE_SIZE_ + _PLAYER_CONSTANT_Z_TRANSLATION_,
        .box_world_pos_camera = camera,
        .x = 0,
        .y = 0,
        
        .string = tile_type->give_alert_string
    };

    game_struct.players[player].tiles[at_tile].type = TILE_TYPE_EMPTY;
    game_struct.players[player].tiles[at_tile].cooldown_timer        = 0;
    game_struct.players[player].tiles[at_tile].curent_cooldown_timer = 0;
    game_struct.players[player].tiles[at_tile].destroyed_effect_time_to_live = _TILE_DESTROYED_EFFECT_TIME_;

    if (player == 0 && play_type != PLAY_TYPE_AGAINST_AI) {
        nf_packet_t packet = (nf_packet_t){
            .packet_len = 3,
            .packet_type = CLIENT_UPDATE,
            .client_id = my_client_id,
            .packet_body[0] = GAME_UPDATE_DEMOLISH
        };
        memcpy(&(packet.packet_body[1]), &at_tile, sizeof(at_tile));
        packet.packet_len += sizeof(at_tile);
        
        if (play_type == PLAY_TYPE_AGAINST_HOST) {
            send_update_packet(packet);
        }
        if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
            send_update_packet_as_server(packet);
        }
    }
}

void player_1_turn() {
    if (play_type == PLAY_TYPE_AGAINST_AI) {
        player_1_ai_turn();
    }else {
        player_1_connected_turn();
    }
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
void init_game_struct() {
    game_struct.player_turn = rand() % 2;
    game_struct.game_ended = 0;

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
}
void generate_starting_tile_positions(int32_t tile_positions[4]) {
    tile_positions[0] = rand() % (_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_);
    do {
        tile_positions[1] = rand() % (_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_);
    } while (tile_positions[1] == tile_positions[0]);

    tile_positions[2] = rand() % (_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_);
    do {
        tile_positions[3] = rand() % (_PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_);
    } while (tile_positions[3] == tile_positions[2]);
}
void game_struct_set_starting_tiles(int32_t tile_positions[4]) {
    game_struct.players[0].tiles[tile_positions[0]].type = TILE_TYPE_HOUSE;
    game_struct.players[0].tiles[tile_positions[0]].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[0].tiles[tile_positions[0]].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[0].tiles[tile_positions[1]].type = TILE_TYPE_FOREST;
    game_struct.players[0].tiles[tile_positions[1]].cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    game_struct.players[0].tiles[tile_positions[1]].curent_cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    game_struct.players[1].tiles[tile_positions[2]].type = TILE_TYPE_HOUSE;
    game_struct.players[1].tiles[tile_positions[2]].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[1].tiles[tile_positions[2]].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[1].tiles[tile_positions[3]].type = TILE_TYPE_FOREST;
    game_struct.players[1].tiles[tile_positions[3]].cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
    game_struct.players[1].tiles[tile_positions[3]].curent_cooldown_timer = tile_type_properties[TILE_TYPE_FOREST].give_cooldown;
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

void exit_game_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
        #ifdef DEBUG_SOFT_MODE
        printf("exiting game - closing server\n");
        #endif
        close_server();
    }
    if (play_type == PLAY_TYPE_AGAINST_HOST) {
        #ifdef DEBUG_SOFT_MODE
        printf("exiting game - exiting server\n");
        #endif
        exit_server();
    }
    switch_page(PAGE_MAIN_MENU);
}
void build_specific_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    int32_t tile_type_id = button_id;

    if (build_at_tile(0, tile_type_id, selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x) == 0) {
        // not enough resources
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }

    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void build_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
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
    int32_t new_ui_list_id = new_ui_list_child(
        ui_list_id,
        get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding,
        0,
        1,
        &nine_slice2
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &build_specific_button_callback,
                .string = "House   ",
                .font = &letters_font,
                .padding = 2,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = tile_type_properties[0].build_info_string,
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &build_specific_button_callback,
                .string = "Barracks",
                .font = &letters_font,
                .padding = 2,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = tile_type_properties[1].build_info_string,
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &build_specific_button_callback,
                .string = "Field   ",
                .font = &letters_font,
                .padding = 2,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = tile_type_properties[2].build_info_string,
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &build_specific_button_callback,
                .string = "Mine    ",
                .font = &letters_font,
                .padding = 2,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = tile_type_properties[3].build_info_string,
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &build_specific_button_callback,
                .string = "Forest  ",
                .font = &letters_font,
                .padding = 2,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = tile_type_properties[3].build_info_string,
                .info_string_font = &letters_font,
                .info_string_padding = 3,
                .info_string_nslice = &nine_slice1
            }
        }
    );
    make_ui_list_safe(new_ui_list_id);
}
void attack_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    if (attack_tile(1, selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x) == 0) {
        // not enough resources
        add_error_alert_at_cursor("Not enough resources");
        make_ui_list_safe(ui_list_id);
        return;
    }
    
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void demolish_sure_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
        add_error_alert_at_cursor("The game has ended");
        close_unperm_ui_lists();
        // unselect tile
        selected_tile.x = -1;
        selected_tile.y = -1;
        return;
    }

    demolish_tile(0, selected_tile.y*_PLAYER_GRID_WIDTH_ + selected_tile.x);
    
    close_unperm_ui_lists();
    // unselect tile
    selected_tile.x = -1;
    selected_tile.y = -1;
}
void demolish_button_callback(int32_t ui_list_id, int32_t button_id) {
    if (game_struct.game_ended == 1) {
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
    int32_t new_ui_list_id = new_ui_list_child(
        ui_list_id,
        get_ui_list_width(ui_list_id) + ui_lists[ui_list_id].padding + 2 - 1,
        ui_lists[ui_list_id].padding - 2,
        2,
        &nine_slice1
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &demolish_sure_button_callback,
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