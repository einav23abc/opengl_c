#ifndef PAGE_IN_GAME_AGAINST_CONNECTED_H
#define PAGE_IN_GAME_AGAINST_CONNECTED_H

#include "page.h"

enum GAME_UPDATE_TYPE {
    GAME_UPDATE_STATE_PACKET_ARRIVED,
    GAME_UPDATE_BUILD,
    GAME_UPDATE_ATTACK,
    GAME_UPDATE_DEMOLISH,
    GAME_UPDATE_END_TURN
};

extern int32_t my_client_id;

void player_1_connected_turn();

#endif