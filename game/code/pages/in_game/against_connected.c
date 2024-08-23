#include "against_connected.h"

void player_1_connected_turn() {

}

// <netframe functions>

nf_packet_t generate_state_packet() {
    // started hosting and a client connected
    init_game_struct();
    nf_packet_t packet = (nf_packet_t){
        .packet_len = 2,
        .client_id = -1,
        .packet_type = SERVER_STATE
    };
}

void parse_state_packet(nf_packet_t packet) {

}

void parse_update_packet(nf_packet_t packet) {

}

void handle_client_connect(int32_t client_id) {

}

void handle_client_disconnect(int32_t client_id) {

}

void handle_disconnect_as_client() {

}
// </netframe functions>