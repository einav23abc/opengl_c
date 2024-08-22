#include "against_ai.h"

ai_action_func_t ai_build_functions[_AI_BUILD_FUNCTIONS_AMOUNT_] = {
    &player_ai_build_first_least_func,
    &player_ai_build_least_func
};
ai_action_func_t ai_attack_functions[_AI_ATTACK_FUNCTIONS_AMOUNT_] = {
    &player_ai_attack_least_func,
    &player_ai_attack_most_func
};


ai_t current_ai;
int32_t ai_action_cooldown;
int8_t player1_ai_played;


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
    
    // find space to build
    int32_t new_tile_grid_index = get_random_empty_tile_position(game_struct.player_turn);
    // handle no space
    if (new_tile_grid_index == -1) return 0;

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
        if (build_at_tile(game_struct.player_turn, tile_type, new_tile_grid_index) == 0) continue;

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

    // find space to build
    int32_t new_tile_grid_index = get_random_empty_tile_position(game_struct.player_turn);
    // handle no space
    if (new_tile_grid_index == -1) return 0;

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
        if (build_at_tile(game_struct.player_turn, tile_type, new_tile_grid_index) == 0) continue;

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