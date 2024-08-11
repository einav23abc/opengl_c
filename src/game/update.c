#include "game.h"

void update() {
    if (in_game) {
        update_game();
        return;
    }

    load_game_update();
    return;
}

void update_game() {
    frames += delta_frames;

    players_update();
    player_camera_update();
    sun_shadow_map_update();
}

void load_game_update() {
    // wait for load_game to finish
    
    default_camera->viewport_w = window_drawable_width;
    default_camera->viewport_h = window_drawable_height;

    int32_t load_game_exit_code = get_thread_exit_code(load_game_thread);
    if (load_game_exit_code == STILL_ACTIVE) return;
    
    if (load_game_exit_code != 0) {
        engine_clean();
        return;
    }
    
    join_thread(load_game_thread);
    default_camera->active = 0;
    in_game = 1;

    bind_mesh(man_mesh);
    bind_mesh(cube_mesh);

    // attempt to join server if fails, open it
    is_server_host = 0;
    client_id = join_server("10.100.102.22");
    player_id = client_id;
    if (client_id < 0) {
        // failed to join server
        is_server_host = 1 - open_server_local();
        client_id = -1;
        player_id = _SERVER_HOST_PLAYER_ID_;
        if (is_server_host == 0) {
            printf("failed to join server and host server\n");
            exit_thread(1);
        }
        printf("hosting server\n");
    }
    init_player_data(player_id);
    player = &(players[player_id]);
}

void players_update() {
    for (int32_t i = 0; i < _PLAYERS_MAX_AMOUNT_; i++) {
        // skip non connected players
        if (players[i].connected == 0) continue;

        // stop from falling off the floor
        if (players[i].cube.y < -60) {
            players[i].cube.y = 100;
            players[i].vy = 0;
        }
        
        // update vx and vz (for my player only)
        if (player_id == i) {
            players[i].vx = 0;
            players[i].vz = 0;
            
            // move left right (set velocity)
            if (keys[SDL_SCANCODE_A]) {
                players[i].vx += 2*cos(player_camera->ry+M_PI);
                players[i].vz += 2*sin(player_camera->ry+M_PI);
            }
            if (keys[SDL_SCANCODE_D]) {
                players[i].vx += 2*cos(player_camera->ry);
                players[i].vz += 2*sin(player_camera->ry);
            }
            
            // move in out (set velocity)
            if (keys[SDL_SCANCODE_S]) {
                players[i].vx += 2*cos(player_camera->ry+M_PI*1.5);
                players[i].vz += 2*sin(player_camera->ry+M_PI*1.5);
            }
            if (keys[SDL_SCANCODE_W]) {
                players[i].vx += 2*cos(player_camera->ry+M_PI*0.5);
                players[i].vz += 2*sin(player_camera->ry+M_PI*0.5);
            }
        }

        // gravity
        players[i].vy -= 0.25*delta_frames;
        // jump (for my player only)
        if (player_id == i) {
            if (keys[SDL_SCANCODE_SPACE] && players[i].can_jump_buffer > 0) {
                players[i].vy = 5;
                players[i].can_jump_buffer = 0;
            }
        }

        // move
        players[i].cube.x += players[i].vx*delta_frames;
        players[i].cube.y += players[i].vy*delta_frames;
        players[i].cube.z += players[i].vz*delta_frames;
        
        // update ry appropriate to move direction
        if (players[i].vz != 0 || players[i].vx != 0) {
            while (players[i].cube.ry >= M_PI*2) {players[i].cube.ry -= M_PI*2;}
            while (players[i].cube.ry < 0) {players[i].cube.ry += M_PI*2;}

            float  goal_ry = atan(players[i].vz/players[i].vx)+M_PI*1.5;
            if (players[i].vx < 0) {
                goal_ry += M_PI;
            }

            while (goal_ry >= M_PI*2) {goal_ry -= M_PI*2;}
            while (goal_ry < 0) {goal_ry += M_PI*2;}
            
            // +-2PI for smallest diffrence
            if (abs(players[i].cube.ry-(goal_ry-M_PI*2)) < abs(players[i].cube.ry-goal_ry)) {
                goal_ry -= M_PI*2;
            }
            if (abs(players[i].cube.ry-(goal_ry+M_PI*2)) < abs(players[i].cube.ry-goal_ry)) {
                goal_ry += M_PI*2;
            }

            float delta_ry = 0.15 * delta_frames;
            players[i].cube.ry = players[i].cube.ry*(1.0-delta_ry) + goal_ry*(delta_ry);
        }

        cube_update_aabb(&(players[i].cube));

        // update can_jump_buffer (for my player only)
        if (player_id == i) {
            if (players[i].can_jump_buffer > 0) {
                players[i].can_jump_buffer -= 1*delta_frames;
            }
        }

        // <collision>
        vec3_t resolution;
        for (uint32_t j = 0; j < CUBES_AMOUNT; j++) {
            resolution = sat_cube_collision(&(players[i].cube), &(cubes[j]));
            if (resolution.x != 0 || resolution.y != 0 || resolution.z != 0) {
                players[i].cube.x += resolution.x;
                players[i].cube.y += resolution.y;
                players[i].cube.z += resolution.z;
            }

            // reset can_jump_buffer and vy if on floor
            if (resolution.y > 0) {
                players[i].can_jump_buffer = 10;
                // hit the floor - reset vy
                players[i].vy = 0;
            }
        }
        // </collision>

        // <animating>
        if (players[i].current_anim != man_anim_run && (players[i].vx || players[i].vz)) {
            players[i].last_anim = players[i].current_anim;
            players[i].current_anim = man_anim_run;
            players[i].last_anim_frame = players[i].current_anim_frame;
            players[i].current_anim_frame = 0;
            players[i].anim_transition_frame = 0;
        }
        if (players[i].current_anim != man_anim_t_pose && (players[i].vx == 0 && players[i].vz == 0)) {
            players[i].last_anim = players[i].current_anim;
            players[i].current_anim = man_anim_t_pose;
            players[i].last_anim_frame = players[i].current_anim_frame;
            players[i].current_anim_frame = 0;
            players[i].anim_transition_frame = 0;
        }
        // players[i].last_anim_frame += delta_frames;
        players[i].current_anim_frame += delta_frames;
        players[i].anim_transition_frame += delta_frames;
        // </animating>

        // send update packets (for my player only)
        if (player_id == i) {
            // sending update packets in {_UPDATE_PACKET_FRAME_INTERVALS_} frame intervals
            update_packet_delta_frames += delta_frames;
            if (update_packet_delta_frames > _UPDATE_PACKET_FRAME_INTERVALS_) {
                update_packet_delta_frames = fmod(update_packet_delta_frames, _UPDATE_PACKET_FRAME_INTERVALS_);

                // send update packet
                client_packet_t update_packet = (client_packet_t) {
                    .packet_len = 1+sizeof(update_packet_body_t),
                    .packet_type = CLIENT_UPDATE
                };
                *((update_packet_body_t*)&update_packet.packet_body) = (update_packet_body_t){
                    .x = players[i].cube.x,
                    .y = players[i].cube.y,
                    .z = players[i].cube.z,
                    .rx = players[i].cube.rx,
                    .ry = players[i].cube.ry,
                    .rz = players[i].cube.rz,
                    .vx = players[i].vx,
                    .vy = players[i].vy,
                    .vz = players[i].vz
                };
                
                if (is_server_host) {
                    send_update_packet_as_server(update_packet);
                }else {
                    send_update_packet(update_packet);
                }
            }
        }
    }
}

void player_camera_update() {
    // X axis rotation
    if (keys[SDL_SCANCODE_KP_2]) {
        player_camera->rx -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_8]) {
        player_camera->rx += 0.05*delta_frames;
    }

    // Z axis rotation
    if (keys[SDL_SCANCODE_KP_7]) {
        player_camera->rz -= 0.01*delta_frames;
    }else if (keys[SDL_SCANCODE_KP_9]) {
        player_camera->rz += 0.01*delta_frames;
    }else {
        player_camera->rz = 0;
    }

    // Y axis rotation
    if (keys[SDL_SCANCODE_KP_6]) {
        player_camera->ry -= 0.05*delta_frames;
    }
    if (keys[SDL_SCANCODE_KP_4]) {
        player_camera->ry += 0.05*delta_frames;
    }

    // follow the player
    player_camera->x = player->cube.x      - 60*cos(player_camera->ry+M_PI*0.5)*cos(player_camera->rx);
    player_camera->y = player->cube.y + 20 - 60*sin(player_camera->rx);
    player_camera->z = player->cube.z      - 60*sin(player_camera->ry+M_PI*0.5)*cos(player_camera->rx);

    // perspective <-> orthographic togle
    if (keys[SDL_SCANCODE_P] == 1) {
        if (player_camera->is_prespective) {
            player_camera->is_prespective = 0;
            player_camera->near = -32000;
        } else {
            player_camera->is_prespective = 1;
            player_camera->near = 0.01;
        }
    }
}

void sun_shadow_map_update() {
    // follow the player
    sun_shadow_map_camera->x = player->cube.x;
    sun_shadow_map_camera->y = player->cube.y;
    sun_shadow_map_camera->z = player->cube.z;

    // int32_t time = SDL_GetTicks()

    sun_shadow_map_camera->ry += 0.002*delta_frames;
    sun_vector_x = cos(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
    sun_vector_y = sin(sun_shadow_map_camera->rx);
    sun_vector_z = sin(sun_shadow_map_camera->ry+M_PI*0.5)*cos(sun_shadow_map_camera->rx);
}