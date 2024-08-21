#include "game.h"

uint8_t init() {
    game_meshes_init();
    game_shaders_init();
    game_textures_init();
    game_sounds_init();

    // <cameras>
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

    // <letters_font>
        letters_font = (font_t){
            .font_texture = letters_font_texture,
            .letters_in_row = 16,
            .letters_in_col = 10,
            .letter_width = 6,
            .letter_height = 12
        };
    // </letters_font>

    // <big_letters_font>
        big_letters_font = (font_t){
            .font_texture = big_letters_font_texture,
            .letters_in_row = 16,
            .letters_in_col = 10,
            .letter_width = 12,
            .letter_height = 24
        };
    // </big_letters_font>

    // <nine_slices>
        nine_slice1 = (nine_slice_t){
            .texture_width = 16,
            .texture_height = 16,
            .padding = 3,
            .texture = nine_slice1_texture
        };
        
        nine_slice2 = (nine_slice_t){
            .texture_width = 16,
            .texture_height = 16,
            .padding = 3,
            .texture = nine_slice2_texture
        };
        
        nine_slice3 = (nine_slice_t){
            .texture_width = 16,
            .texture_height = 16,
            .padding = 3,
            .texture = nine_slice3_texture
        };
    // </nine_slices>

    // <tile_type_properties>
        tile_type_properties[TILE_TYPE_HOUSE] = (tile_type_t){
            .cost = (resources_t){
                .wood = 1,
                .stone = 2,
                .wheat = 1,
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
                .wood = 1,
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
                build_info_string[c+4] = '\x11';
                build_info_string[c+5] = '\x12';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                build_info_string[c+4] = '\x13';
                build_info_string[c+5] = '\x14';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                build_info_string[c+4] = '\x15';
                build_info_string[c+5] = '\x16';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                build_info_string[c+4] = '\x17';
                build_info_string[c+5] = '\x18';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '+';
                build_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                build_info_string[c+4] = '\x19';
                build_info_string[c+5] = '\x1a';
                build_info_string[c+6] = '\n';
                c += 7;
            }

            build_info_string[c] = '\n';
            c += 1;

            strcpy(&(build_info_string[c]), "Costs:\n");
            c += strlen("costs:\n");
            
            if (tile_type_properties[i].cost.population > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.population;
                build_info_string[c+4] = '\x11';
                build_info_string[c+5] = '\x12';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.wheat > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wheat;
                build_info_string[c+4] = '\x13';
                build_info_string[c+5] = '\x14';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.wood > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.wood;
                build_info_string[c+4] = '\x15';
                build_info_string[c+5] = '\x16';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.stone > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.stone;
                build_info_string[c+4] = '\x17';
                build_info_string[c+5] = '\x18';
                build_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].cost.soldiers > 0) {
                build_info_string[c  ] = '*';
                build_info_string[c+1] = ' ';
                build_info_string[c+2] = '-';
                build_info_string[c+3] = '0' + tile_type_properties[i].cost.soldiers;
                build_info_string[c+4] = '\x19';
                build_info_string[c+5] = '\x1a';
                build_info_string[c+6] = '\n';
                c += 7;
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
                give_alert_string[c+2] = '\x11';
                give_alert_string[c+3] = '\x12';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wheat;
                give_alert_string[c+2] = '\x13';
                give_alert_string[c+3] = '\x14';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.wood > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.wood;
                give_alert_string[c+2] = '\x15';
                give_alert_string[c+3] = '\x16';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.stone > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.stone;
                give_alert_string[c+2] = '\x17';
                give_alert_string[c+3] = '\x18';
                give_alert_string[c+4] = '\n';
                c += 5;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                give_alert_string[c  ] = '+';
                give_alert_string[c+1] = '0' + tile_type_properties[i].give.soldiers;
                give_alert_string[c+2] = '\x19';
                give_alert_string[c+3] = '\x1a';
                give_alert_string[c+4] = '\n';
                c += 5;
            }

            // remove newline and add null terminator
            give_alert_string[c-1] = '\0';

            strcpy(tile_type_properties[i].give_alert_string, give_alert_string);
        }

        // demolish_info_string
        for (uint32_t i = 0; i < _TILE_TYPES_AMOUNT_; i++) {
            char demolish_info_string[_TILE_DEMOLISH_INFO_STRING_MAX_LENGTH_];
            uint32_t c = 0;
            
            strcpy(&(demolish_info_string[c]), "Destroy this building\n");
            c += strlen("Destroy this building\n");

            if (tile_type_properties[i].give.population > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.population;
                demolish_info_string[c+4] = '\x11';
                demolish_info_string[c+5] = '\x12';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wheat > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wheat;
                demolish_info_string[c+4] = '\x13';
                demolish_info_string[c+5] = '\x14';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.wood > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.wood;
                demolish_info_string[c+4] = '\x15';
                demolish_info_string[c+5] = '\x16';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.stone > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.stone;
                demolish_info_string[c+4] = '\x17';
                demolish_info_string[c+5] = '\x18';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }
            if (tile_type_properties[i].give.soldiers > 0) {
                demolish_info_string[c  ] = '*';
                demolish_info_string[c+1] = ' ';
                demolish_info_string[c+2] = '+';
                demolish_info_string[c+3] = '0' + tile_type_properties[i].give.soldiers;
                demolish_info_string[c+4] = '\x19';
                demolish_info_string[c+5] = '\x1a';
                demolish_info_string[c+6] = '\n';
                c += 7;
            }

            // remove newline and add null terminator
            demolish_info_string[c-1] = '\0';

            strcpy(tile_type_properties[i].demolish_info_string, demolish_info_string);
        }
    // </tile_type_properties>


    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].active = 0;
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;

    enter_main_menu();

    return 0;
}