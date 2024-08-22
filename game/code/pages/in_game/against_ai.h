#ifndef PAGE_IN_GAME_AGAINST_AI_H
#define PAGE_IN_GAME_AGAINST_AI_H

#include "page.h"

typedef int32_t(*ai_action_func_t)(void);

typedef struct {
    int32_t tile_build_priority;
    int32_t tile_build_priority_strength; // 0 = always, 1 = 1/2 the time, 2 = 1/3 of the time
    ai_action_func_t build_func;
    int32_t tile_attack_priority;
    ai_action_func_t attack_func;
} ai_t;


extern ai_action_func_t ai_build_functions[_AI_BUILD_FUNCTIONS_AMOUNT_];
extern ai_action_func_t ai_attack_functions[_AI_ATTACK_FUNCTIONS_AMOUNT_];

extern ai_t current_ai;
extern int32_t ai_action_cooldown;
extern int8_t player1_ai_played;


void player_1_ai_turn();
int32_t player_ai_build_first_least_func();
int32_t player_ai_build_least_func();
int32_t player_ai_attack_least_func();
int32_t player_ai_attack_most_func();

#endif