#include "against_connected.h"


int32_t my_client_id;


void player_1_connected_turn() {

}

// <netframe functions>

nf_packet_t generate_state_packet() {
    // started hosting and a client connected - start playing

    // go to joining game page until a state packet receive confirmation arrives
    // play type was set in open_lan/enter.c
    switch_page(PAGE_JOINING_GAME);

    // create state packet of game_struct
    init_game_struct();
    int32_t tile_positions[4] = {
        get_random_empty_tile_position(0),
        get_random_empty_tile_position(0),
        get_random_empty_tile_position(1),
        get_random_empty_tile_position(1)
    };
    game_struct_set_starting_tiles(
        tile_positions[0],
        tile_positions[1],
        tile_positions[2],
        tile_positions[3]
    );
    nf_packet_t packet = (nf_packet_t){
        .packet_len = 2,
        .packet_type = SERVER_STATE,
        .client_id = -1
    };
    memcpy(packet.packet_body, tile_positions, sizeof(tile_positions));
    packet.packet_len += sizeof(tile_positions);
    return packet;
}

void parse_state_packet(nf_packet_t packet) {
    // joined a host - start playing

    // set game_struct according to packet
    init_game_struct();
    int32_t tile_positions[4];
    memcpy(tile_positions, packet.packet_body, sizeof(tile_positions));
    game_struct_set_starting_tiles(
        tile_positions[0],
        tile_positions[1],
        tile_positions[2],
        tile_positions[3]
    );
    // flip players
    player_t tmp_player = game_struct.players[0];
    game_struct.players[0] = game_struct.players[1];
    game_struct.players[1] = tmp_player;
    game_struct.player_turn = !game_struct.player_turn;

    // send state packet arried confirmation
    nf_packet_t update_packet = (nf_packet_t){
        .packet_len = 3,
        .packet_type = CLIENT_UPDATE,
        .client_id = my_client_id,
        .packet_body[0] = GAME_UPDATE_STATE_PACKET_ARRIVED
    };
    send_update_packet(*((nf_packet_t*)&update_packet));
    #ifdef DEBUG_SOFT_MODE
    printf("sent state packet arrived confirmation\n");
    #endif

    // go to game page
    // play type was set in join_lan/enter.c
    switch_page(PAGE_IN_GAME);
}

void parse_update_packet(nf_packet_t packet) {
    if (packet.client_id == my_client_id) return;

    switch (packet.packet_body[0]) {
        case GAME_UPDATE_STATE_PACKET_ARRIVED: {
            #ifdef DEBUG_SOFT_MODE
            printf("received state packet arrived confirmation\n");
            #endif
            // state packet arived confiramtion
            // play type was set in open_lan/enter.c
            switch_page(PAGE_IN_GAME);
            break;
        }

        case GAME_UPDATE_BUILD: {
            #ifdef DEBUG_SOFT_MODE
            printf("received update build packet\n");
            #endif

            int32_t tile_type_id;
            int32_t at_tile;
            
            memcpy(&tile_type_id, &(packet.packet_body[1]),                      sizeof(tile_type_id));
            memcpy(&at_tile,      &(packet.packet_body[1+sizeof(tile_type_id)]), sizeof(at_tile));

            build_at_tile(1, tile_type_id, at_tile);
            break;
        }

        case GAME_UPDATE_ATTACK: {
            #ifdef DEBUG_SOFT_MODE
            printf("received update attack packet\n");
            #endif

            int32_t at_tile;
            
            memcpy(&at_tile, &(packet.packet_body[1]), sizeof(at_tile));

            attack_tile(0, at_tile);
            break;
        }

        case GAME_UPDATE_DEMOLISH: {
            #ifdef DEBUG_SOFT_MODE
            printf("received update demolish packet\n");
            #endif

            int32_t at_tile;
            
            memcpy(&at_tile, &(packet.packet_body[1]), sizeof(at_tile));

            demolish_tile(1, at_tile);
            break;
        }

        case GAME_UPDATE_END_TURN: {
            #ifdef DEBUG_SOFT_MODE
            printf("received update end turn packet\n");
            #endif

            switch_turn();
            break;
        }
    }
}

void handle_client_disconnect(int32_t client_id) {
    if (page == PAGE_MAIN_MENU || next_page == PAGE_MAIN_MENU) return;

    if (play_type == PLAY_TYPE_AGAINST_CLIENT) {
        switch_page(PAGE_DISCONNECTED_FROM_CLIENT);
        #ifdef DEBUG_SOFT_MODE
        printf("client disconnected, closing server\n");
        #endif
        close_server();
    }
}

void handle_disconnect_as_client() {
    if (page == PAGE_MAIN_MENU || next_page == PAGE_MAIN_MENU) return;

    if (play_type == PLAY_TYPE_AGAINST_HOST) {
        switch_page(PAGE_DISCONNECTED_FROM_HOST);
        #ifdef DEBUG_SOFT_MODE
        printf("server disconnected\n");
        #endif
    }
}
// </netframe functions>