#include "game.h"

uint8_t init() {
    // <meshes;>
        if(1){
        float vertices_position_arr[] = {
            // front
            0,0,0,
            0,1,0,
            1,1,0,
            1,0,0,
            // back
            0,0,1,
            0,1,1,
            1,1,1,
            1,0,1,
            // bottom
            0,0,0,
            0,0,1,
            1,0,1,
            1,0,0,
            // top
            0,1,0,
            0,1,1,
            1,1,1,
            1,1,0,
            // left
            0,0,0,
            0,1,0,
            0,1,1,
            0,0,1,
            // right
            1,0,0,
            1,1,0,
            1,1,1,
            1,0,1,
        };
        float vertices_texcoord_arr[] = {
            // front
            0,1,
            0,0,
            1,0,
            1,1,
            // back
            1,1,
            1,0,
            0,0,
            0,1,
            // bottom
            0,1,
            0,0,
            1,0,
            1,1,
            // top
            0,1,
            0,0,
            1,0,
            1,1,
            // left
            1,1,
            1,0,
            0,0,
            0,1,
            // right
            0,1,
            0,0,
            1,0,
            1,1,
        };
        float vertices_normal_arr[] = {
            // front
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            // back
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            // bottom
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            // top
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            // left
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            // right
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
        };
        vbo_data_t vbo_datas_arr[3] = {
            {
                .data_arr_size = sizeof(vertices_position_arr),
                .data_arr = (void*)vertices_position_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_texcoord_arr),
                .data_arr = (void*)vertices_texcoord_arr,
                .size = 2,
                .type = GL_FLOAT,
                .stride = 2*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_normal_arr),
                .data_arr = (void*)vertices_normal_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            }
        };
        
        uint32_t indices_array[] = {
            // front
            1, 0, 2,
            2, 0, 3,
            // back
            4, 5, 6,
            4, 6, 7,
            // bottom
            8, 9,10,
            8,10,11,
            // top
            13,12,14,
            14,12,15,
            // left
            16,17,18,
            16,18,19,
            // right
            21,20,22,
            22,20,23,
        };

        cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 0);
        // bind_mesh(cube_mesh);
        
        for (uint32_t i = 0; i <sizeof(vertices_position_arr)/sizeof(float); i++) vertices_position_arr[i] -= 0.5;
        centered_cube_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 36, 0);
        };

        // tile_effect_mesh
        if(1){
        float vertices_position_arr[] = {
            // front
            0,0,0,
            0,1,0,
            1,1,0,
            1,0,0,
            // back
            0,0,1,
            0,1,1,
            1,1,1,
            1,0,1,
            // bottom
            0,0,0,
            0,0,1,
            1,0,1,
            1,0,0,
            // top
            0,1,0,
            0,1,1,
            1,1,1,
            1,1,0,
            // left
            0,0,0,
            0,1,0,
            0,1,1,
            0,0,1,
            // right
            1,0,0,
            1,1,0,
            1,1,1,
            1,0,1,
        };
        float vertices_texcoord_arr[] = {
            // front
            0,1,
            0,0,
            1,0,
            1,1,
            // back
            1,1,
            1,0,
            0,0,
            0,1,
            // bottom
            0,1,
            0,0,
            1,0,
            1,1,
            // top
            0,1,
            0,0,
            1,0,
            1,1,
            // left
            1,1,
            1,0,
            0,0,
            0,1,
            // right
            0,1,
            0,0,
            1,0,
            1,1,
        };
        float vertices_normal_arr[] = {
            // front
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            // back
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            // bottom
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            // top
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            // left
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            // right
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
        };
        vbo_data_t vbo_datas_arr[3] = {
            {
                .data_arr_size = sizeof(vertices_position_arr),
                .data_arr = (void*)vertices_position_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_texcoord_arr),
                .data_arr = (void*)vertices_texcoord_arr,
                .size = 2,
                .type = GL_FLOAT,
                .stride = 2*sizeof(float),
                .divisor = 0
            },
            {
                .data_arr_size = sizeof(vertices_normal_arr),
                .data_arr = (void*)vertices_normal_arr,
                .size = 3,
                .type = GL_FLOAT,
                .stride = 3*sizeof(float),
                .divisor = 0
            }
        };
        
        uint32_t indices_array[] = {
            // front
            1, 0, 2,
            2, 0, 3,
            // back
            4, 5, 6,
            4, 6, 7,
            // left
            16,17,18,
            16,18,19,
            // right
            21,20,22,
            22,20,23,
        };

        tile_effect_mesh = generate_mesh(vbo_datas_arr, 3, indices_array, 24, 0);
        }
    // </meshes>

    // <rect_plane_mesh>
        rect_plane_mesh = generate_2d_quad_mesh(0, 1, 0, 1, 0, 1, 0, 1);
    // </rect_plane_mesh>

    // <cameras>
        camera_pos = (vec3_t){
            .x = -_TILE_SIZE_*0.5,
            .y = _SCALE_AXIS_POINT_Y_,
            .z = -_TILE_SIZE_
        };
        camera = create_camera(
            0, 0, 0,
            M_PI*1.8, -M_PI*0.15, 0,
            380, 260, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );

        ui_camera = create_camera(
            _OUTPORT_WIDTH_*0.5, _OUTPORT_HEIGHT_*0.5, 0,
            0, 0, 0,
            _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );
        ui_camera->active = 0;
    // </cameras>

    // <sun shadow map>
        sun_shadow_map_camera = create_camera(
            0, 0, 0,
            M_PI*1.8, M_PI*1.1, 0,
            1080, 1080, 1600,
            -32000, 32000,
            0, 45,
            0, 0, 3240, 3240
        );
    // </sun shadow map>


    // <fbos>
        outport_fbo = create_fbo(_OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1, GL_RGB, 4);

        sun_shadow_map_fbo = create_fbo(3240, 3240, 0, 0, 2);
    // </fbos>

    // <textures>
        floor_texture = load_texture("./game/textures/floor.png");
        global_texture = load_texture("./game/textures/global_texture.png");
        // save_surface_to_c_file("./game/textures/global_texture.png", "global_texture_surface", "./game/textures/global_texture_surface.c");
        
        // <letters_font>
            letters_font = (font_t){
                .font_texture = load_texture("./game/textures/font.png"),
                .letters_in_row = 16,
                .letters_in_col = 10,
                .letter_width = 6,
                .letter_height = 12
            };
        // </letters_font>
    // </textures>

    // <shaders>
        global_shader = create_shader_from_files(
            "./game/shaders/global.vert",
            "./game/shaders/global.frag",
            "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
            "u_position\0u_scale\0u_quat_rotation\0u_camera_position\0u_sun_vector\0u_sun_shadow_map_wvp_mat\0u_sun_shadow_map_texture", 7
        );

        ui_shader = create_shader_from_files(
            "./game/shaders/ui.vert",
            "./game/shaders/ui.frag",
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation", 3
        );

        font_shader = create_shader_from_files(
            "./game/shaders/font.vert",
            "./game/shaders/font.frag",
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation\0u_text_row_length\0u_text_row\0u_font_data\0u_color", 7
        );

        cooldown_billboards_shader = create_shader_from_files(
            "./game/shaders/cooldown.vert",
            "./game/shaders/cooldown.frag",
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_rads", 3
        );

        tile_effect_shader = create_shader_from_files(
            "./game/shaders/tile_effect.vert",
            "./game/shaders/tile_effect.frag",
            "in_vertex_position\0in_vertex_texcoord", 2,
            "u_position\0u_scale\0u_quat_rotation\0u_color\0u_speed\0u_freq\0u_time", 7
        );
        
        sun_shadow_map_shader = create_shader_from_files(
            "./game/shaders/sun_shadow_map.vert",
            "./game/shaders/sun_shadow_map.frag",
            "in_vertex_position\0in_vertex_texcoord\0in_vertex_normal\0in_vertex_joint_id\0in_vertex_joint_wheight", 5,
            "u_position\0u_scale\0u_quat_rotation", 3
        );
    // </shaders>


    // <tile_type_properties>
        tile_type_properties[TILE_TYPE_HOUSE] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 1,
                .wheat = 2,
                .population = 0,
                .soldiers = 0
            },
            .give_cooldown = 3,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 0,
                .population = 1,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_BARRACKS] = (tile_type_t){
            .cost = (resources_t){
                .wood = 0,
                .stone = 2,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 3,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 0,
                .population = 0,
                .soldiers = 1
            }
        };
        tile_type_properties[TILE_TYPE_FIELD] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 0,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 1,
                .population = 0,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_MINE] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 0,
                .stone = 1,
                .wheat = 0,
                .population = 0,
                .soldiers = 0
            }
        };
        tile_type_properties[TILE_TYPE_FOREST] = (tile_type_t){
            .cost = (resources_t){
                .wood = 0,
                .stone = 0,
                .wheat = 1,
                .population = 1,
                .soldiers = 0
            },
            .give_cooldown = 2,
            .give = (resources_t){
                .wood = 1,
                .stone = 0,
                .wheat = 0,
                .population = 0,
                .soldiers = 0
            }
        };

        // build_info_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char build_info_string[_TILE_BUILD_INFO_STRING_MAX_LENGTH_];
            uint32_t c = 0;

            strcpy(&(build_info_string[c]), "costs:\n");
            c += strlen("costs:\n");
            
            if (tile_type_properties[i].cost.population > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.population;
                build_info_string[c+4] = '\1';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].cost.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wheat;
                build_info_string[c+4] = '\2';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].cost.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wood;
                build_info_string[c+4] = '\3';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].cost.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.stone;
                build_info_string[c+4] = '\4';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].cost.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.soldiers;
                build_info_string[c+4] = '\5';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            

            if (tile_type_properties[i].give_cooldown == 1) {
                strcpy(&(build_info_string[c]), "Every turn:\n");
                c += strlen("Every turn:\n");
            }else {
                build_info_string[c  ]  = 'E';
                build_info_string[c+1]  = 'v';
                build_info_string[c+2]  = 'e';
                build_info_string[c+3]  = 'r';
                build_info_string[c+4]  = 'y';
                build_info_string[c+5]  = ' ';
                build_info_string[c+6]  = '0' + tile_type_properties[i].give_cooldown;
                build_info_string[c+7]  = ' ';
                build_info_string[c+8]  = 't';
                build_info_string[c+9]  = 'u';
                build_info_string[c+10] = 'r';
                build_info_string[c+11] = 'n';
                build_info_string[c+12] = 's';
                build_info_string[c+13] = ':';
                build_info_string[c+14] = '\n';
                c += 15;
            }
            
            if (tile_type_properties[i].give.population > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.population;
                build_info_string[c+4] = '\1';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                build_info_string[c+4] = '\2';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                build_info_string[c+4] = '\3';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                build_info_string[c+4] = '\4';
                build_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                build_info_string[c+4] = '\5';
                build_info_string[c+5] = '\n';
                c += 6;
            }

            // remove newline and add null terminator
            build_info_string[c-1] = '\0';

            strcpy(tile_type_properties[i].build_info_string, build_info_string);
        }
        
        // give_alert_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char give_alert_string[_TILE_RESOURCE_GIVE_ALERT_STRING_MAX_LENGTH_];
            uint32_t c = 0;

            if (tile_type_properties[i].give.population > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.population;
                give_alert_string[c+2] = '\1';
                give_alert_string[c+3] = '\n';
                c += 4;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wheat;
                give_alert_string[c+2] = '\2';
                give_alert_string[c+3] = '\n';
                c += 4;
            }
            if (tile_type_properties[i].give.wood > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wood;
                give_alert_string[c+2] = '\3';
                give_alert_string[c+3] = '\n';
                c += 4;
            }
            if (tile_type_properties[i].give.stone > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.stone;
                give_alert_string[c+2] = '\4';
                give_alert_string[c+3] = '\n';
                c += 4;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.soldiers;
                give_alert_string[c+2] = '\5';
                give_alert_string[c+3] = '\n';
                c += 4;
            }

            // remove newline and add null terminator
            give_alert_string[c-1] = '\0';

            strcpy(tile_type_properties[i].give_alert_string, give_alert_string);
        }

        // demolish_info_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char demolish_info_string[_TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_];
            uint32_t c = 0;
            
            strcpy(&(demolish_info_string[c]), "destroy this building\n");
            c += strlen("destroy this building\n");

            if (tile_type_properties[i].give.population > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.population;
                demolish_info_string[c+4] = '\1';
                demolish_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                demolish_info_string[c+4] = '\2';
                demolish_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.wood > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                demolish_info_string[c+4] = '\3';
                demolish_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.stone > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                demolish_info_string[c+4] = '\4';
                demolish_info_string[c+5] = '\n';
                c += 6;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                demolish_info_string[c+4] = '\5';
                demolish_info_string[c+5] = '\n';
                c += 6;
            }

            // remove newline and add null terminator
            demolish_info_string[c-1] = '\0';

            strcpy(tile_type_properties[i].demolish_info_string, demolish_info_string);
        }
    // </tile_type_properties>


    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].active = 0;
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;

    init_game();

    return 0;
}

void init_game() {
    selected_tile.x = -1;
    selected_tile.y = -1;
    hovered_tiles[0].x = -1;
    hovered_tiles[0].y = -1;
    hovered_tiles[1].x = -1;
    hovered_tiles[1].y = -1;

    game_struct.player_turn = 0;

    in_cooldowns_translation = 0;

    player_translations_update();
    game_struct.players[0].y_lerp_start_translation = game_struct.players[0].y_translation;
    game_struct.players[0].y_current_translation = game_struct.players[0].y_translation;
    game_struct.players[1].y_lerp_start_translation = game_struct.players[1].y_translation;
    game_struct.players[1].y_current_translation = game_struct.players[1].y_translation;

    game_struct.players[0].translation_lerp_time = 0;
    game_struct.players[1].translation_lerp_time = 0;

    for (uint32_t i = 0; i < _PLAYER_GRID_WIDTH_*_PLAYER_GRID_DEPTH_; i++) {
        game_struct.players[0].tiles[i].type = TILE_TYPE_EMPTY;
        game_struct.players[0].tiles[i].camoflauged = 0;
        game_struct.players[0].tiles[i].cooldown_timer = 0;
        game_struct.players[0].tiles[i].curent_cooldown_timer = 0;
        game_struct.players[1].tiles[i].type = TILE_TYPE_EMPTY;
        game_struct.players[1].tiles[i].camoflauged = 0;
        game_struct.players[1].tiles[i].cooldown_timer = 0;
        game_struct.players[1].tiles[i].curent_cooldown_timer = 0;
    }
    game_struct.players[0].tiles[13].type = TILE_TYPE_HOUSE;
    game_struct.players[0].tiles[13].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[0].tiles[13].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[0].tiles[19].type = TILE_TYPE_MINE;
    game_struct.players[0].tiles[19].cooldown_timer = tile_type_properties[TILE_TYPE_MINE].give_cooldown;
    game_struct.players[0].tiles[19].curent_cooldown_timer = tile_type_properties[TILE_TYPE_MINE].give_cooldown;
    game_struct.players[1].tiles[13].type = TILE_TYPE_HOUSE;
    game_struct.players[1].tiles[13].cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[1].tiles[13].curent_cooldown_timer = tile_type_properties[TILE_TYPE_HOUSE].give_cooldown;
    game_struct.players[1].tiles[19].type = TILE_TYPE_MINE;
    game_struct.players[1].tiles[19].cooldown_timer = tile_type_properties[TILE_TYPE_MINE].give_cooldown;
    game_struct.players[1].tiles[19].curent_cooldown_timer = tile_type_properties[TILE_TYPE_MINE].give_cooldown;

    game_struct.players[0].wheight = 0;
    game_struct.players[0].resources.wood = 2;
    game_struct.players[0].resources.stone = 2;
    game_struct.players[0].resources.wheat = 2;
    game_struct.players[0].resources.soldiers = 0;
    game_struct.players[0].resources.population = 2;
    game_struct.players[1].wheight = 0;
    game_struct.players[1].resources.wood = 2;
    game_struct.players[1].resources.stone = 2;
    game_struct.players[1].resources.wheat = 2;
    game_struct.players[1].resources.soldiers = 0;
    game_struct.players[1].resources.population = 2;

    // next turn ui list
    int32_t ui_list_id = new_ui_list_assign_id();
    ui_lists[ui_list_id] = (ui_list_t){
        .active = 1,
        .permenant = 1,

        .box_pos_from_world_pos = 0,
        .x = _OUTPORT_WIDTH_ - _UI_LIST_PADDING_ - get_str_boxed_size("end_turn", _UI_LIST_BUTTON_HEIGHT_).x,
        .y = _UI_LIST_BUTTON_HEIGHT_ + _UI_LIST_PADDING_,

        .buttons_amount = 1,
        .button_strings = {"end turn"},
        .button_info_strings = {""},
        .button_callbacks = {&switch_turn_button_callback},

        .child_ui_list = -1,
        .parent_ui_list = -1
    };

    page = PAGE_IN_GAME;
}