#ifndef NETFRAME_h
#define NETFRAME_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../sockets/sockets.h"
#include "../threads/threads.h"


#ifndef _SERVER_OUT_PORT_
#define _SERVER_OUT_PORT_ (5050)
#endif
#ifndef _SERVER_IN_PORT_
#define _SERVER_IN_PORT_ (5051)
#endif

#ifndef _PACKET_MAX_LENGTH_
#define _PACKET_MAX_LENGTH_ (128)
#endif
#ifndef _PACKETS_STACK_LENGTH_
#define _PACKETS_STACK_LENGTH_ (8)
#endif

#ifndef _CLIENTS_MAX_AMOUNT_
#define _CLIENTS_MAX_AMOUNT_ (10)
#endif


enum PACKET_TYPE {
    /* Sent from the `client-out-socket`.
     *
     * Completes the connection.
     * 
     * \param packet_type `CLIENT_OUT_SOCKET_CONNECT`.
     * \param packet_body The `client ID` as `int8_t` that was issued by `SERVER_ASSIGNED_CLIENT_ID` to the `client-in-socket`.
     */
    CLIENT_OUT_SOCKET_CONNECT,
    /* Sent from the `client-out-socket`.
     * 
     * \param packet_type `CLIENT_UPDATE`.
     * \param packet_body The update.
     */
    CLIENT_UPDATE,
    
    /* Sent to the `client-in-socket`.
     *
     * Should be passed by the client to the `client-out-socket` for a `CLIENT_OUT_SOCKET_CONNECT` packet.
     *
     * \param packet_type `SERVER_ASSIGNED_CLIENT_ID`.
     * \param client_id The newly connected client's assigned `client ID`.
     * \param packet_body Empty.
     */
    SERVER_ASSIGNED_CLIENT_ID,
    /* Sent to the `client-in-socket`.
     *
     * The server cannot assign a client ID.
     * 
     * The connection is refused.
     * 
     * \param packet_type `SERVER_NO_CLIENT_IDS`.
     * \param client_id `-1`.
     * \param packet_body Empty.
     */
    SERVER_NO_CLIENT_IDS,
    /* Sent to the `client-in-socket`
     *
     * A packet containing data for a reconstruction of the shared object on the client side.
     * 
     * A packet created by `gen_state_packet()` and should be parsed by `parse_state_packet()`, both user defined.
     * 
     * \param packet_type `SERVER_STATE`.
     * \param client_id `-1`.
     * \param packet_body Data set by `gen_state_packet()`.
     */
    SERVER_STATE,
    /* Sent to all `client-in-sockets`.
     *
     * A client X has connected.
     *
     * \param packet_type `SERVER_CLIENT_CONNECTED`.
     * \param client_id The assigned `client ID` of the connected client.
     * \param packet_body Empty.
     */
    SERVER_CLIENT_CONNECTED,
    /* Sent to all `client-in-sockets`.
     *
     * A client X has disconnected.
     *
     * \param packet_type `SERVER_CLIENT_DISCONNECTED`.
     * \param client_id The `client ID` of the disconnected client.
     * \param packet_body Empty.
     */
    SERVER_CLIENT_DISCONNECTED,
    /* Sent to all `client-in-sockets`.
     *
     * A client X has sent an update Y.
     *
     * \param packet_type `SERVER_CLIENT_UPDATE`.
     * \param client_id The `client ID` of the client who sent the update.
     * \param packet_body The update.
     */
    SERVER_CLIENT_UPDATE,
};


#pragma pack(1)
typedef struct {
    int32_t packet_len;
    char packet[_PACKET_MAX_LENGTH_];
} packet_t;
#pragma pack()

#pragma pack(1)
// packets sent from the client
typedef struct {
    int32_t packet_len;

    uint8_t packet_type;
    char packet_body[_PACKET_MAX_LENGTH_-1];
} client_packet_t;
#pragma pack()

#pragma pack(1)
// packets sent from the server
typedef struct {
    int32_t packet_len;

    uint8_t packet_type;
    int8_t client_id; // the client id the server is refrencing
    char packet_body[_PACKET_MAX_LENGTH_-2];
} server_packet_t;
#pragma pack()


/* Opens a server that listens and accept clients that connect with `join_server()`.
 * 
 * The server should be closed with `close_server()`
 * 
 * Upon client connections the server will call to `generate_state_packet()` if it exists, And will send the
 * generated packet to the newly connected client, which will call to `parse_state_packet()` if it exists.
 * The state packet should contain all the data to reconstruct the shared object at the client, this will be
 * done by the `parse_state_packet()` function that is defined by the user. The `generate_state_packet()` should
 * also be defined by the user.
 * 
 * Clients (and the server) can send updates to their data by calling `send_update_packet()`. The server will
 * send the update packet to all clients (including itself) which will call `parse_update_packet()` if it exists.
 * The update packet should contail all the data to convert the clients' shared object to the server's object, this
 * will be done by the `parse_update_packet()` function that is defined by the user.
 * 
 * When a client connects/disconnects the server will tell all clients which will call to `handle_client_connect()` or
 * `handle_client_disconnect()` or `handle_disconnect_as_client()` all defined by the user.
 * 
 * \param ipv4addr The server's IPv4 dotted-decimal address string.
 * 
 * \returns 0 on success or 1 on failure.
 */
int32_t open_server(const char* ipv4addr);

/* Opens a server that listens and accept clients that connect with `join_server()` at a local ipv4 address.
 * 
 * The server should be closed with `close_server()`
 * 
 * Upon client connections the server will call to `generate_state_packet()` if it exists, And will send the
 * generated packet to the newly connected client, which will call to `parse_state_packet()` if it exists.
 * The state packet should contain all the data to reconstruct the shared object at the client, this will be
 * done by the `parse_state_packet()` function that is defined by the user. The `generate_state_packet()` should
 * also be defined by the user.
 * 
 * Clients (and the server) can send updates to their data by calling `send_update_packet()`. The server will
 * send the update packet to all clients (including itself) which will call `parse_update_packet()` if it exists.
 * The update packet should contail all the data to convert the clients' shared object to the server's object, this
 * will be done by the `parse_update_packet()` function that is defined by the user.
 * 
 * When a client connects/disconnects the server will tell all clients which will call to `handle_client_connect()` or
 * `handle_client_disconnect()` or `handle_disconnect_as_client()` all defined by the user.
 * 
 * \returns 0 on success or 1 on failure.
 */
int32_t open_server_local();

/* Closes a server opened by `open_server()` or `open_server_local()`.
 *
 * All connected clients will be disconnected.
 */
void close_server();

/* Join a server as a client.
 *
 * The client can exit the server by calling `exit_server()`.
 * 
 * Upon client connections the server will call to `generate_state_packet()` if it exists, And will send the
 * generated packet to the newly connected client, which will call to `parse_state_packet()` if it exists.
 * The state packet should contain all the data to reconstruct the shared object at the client, this will be
 * done by the `parse_state_packet()` function that is defined by the user. The `generate_state_packet()` should
 * also be defined by the user.
 * 
 * Clients (and the server) can send updates to their data by calling `send_update_packet()`. The server will
 * send the update packet to all clients (including itself) which will call `parse_update_packet()` if it exists.
 * The update packet should contail all the data to convert the clients' shared object to the server's object, this
 * will be done by the `parse_update_packet()` function that is defined by the user.
 * 
 * When a client connects/disconnects the server will tell all clients which will call to `handle_client_connect()` or
 * `handle_client_disconnect()` or `handle_disconnect_as_client()` all defined by the user.
 *
 * \param ipv4addr The server's IPv4 dotted-decimal address string.
 * 
 * \returns The assigned `client_id` on success or `-1` on failure.
 */
int32_t join_server(const char* ipv4addr);

/* Exit a server as a connected client.
 */
void exit_server();

/* Send an update packet to the server as a client.
 *
 * The server will send the update packet to all clients which will call `parse_update_packet()` if it exists.
 * The server will also call `parse_update_packet()` to allow it to act as a client too. The update packet should
 * contail all the data to convert the clients' shared object to the server's object, this will be done by
 * the `parse_update_packet()` function that is defined by the user.
 * 
 * \param packet A client_packet_t created by the client. The packet should have a type of `CLIENT_UPDATE` and
 * should be parsable by `parse_update_packet()` (a user defined function).
 */
void send_update_packet(client_packet_t packet);


/* Send an update packet to the server from the server pretending to be a client with id `-1`.
 *
 * The server will send the update packet to all clients which will call `parse_update_packet()` if it exists.
 * The server will also call `parse_update_packet()` to allow it to act as a client too. The update packet should
 * contail all the data to convert the clients' shared object to the server's object, this will be done by
 * the `parse_update_packet()` function that is defined by the user.
 * 
 * \param packet A client_packet_t created by the server. The packet should have a type of `CLIENT_UPDATE` and
 * should be parsable by `parse_update_packet()` (a user defined function).
 */
void send_update_packet_as_server(client_packet_t packet);

#endif