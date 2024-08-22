#include "game.h"

uint8_t init() {
    game_meshes_init();
    game_shaders_init();
    game_textures_init();
    game_sounds_init();

    // <ui_cameras>
        ui_camera = create_camera(
            _OUTPORT_WIDTH_*0.5, _OUTPORT_HEIGHT_*0.5, 0,
            0, 0, 0,
            _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1600,
            -32000, 32000,
            0, 60,
            0, 0, _OUTPORT_WIDTH_, _OUTPORT_HEIGHT_
        );
        ui_camera->active = 0;
    // </ui_cameras>

    outport_fbo = create_fbo(_OUTPORT_WIDTH_, _OUTPORT_HEIGHT_, 1, GL_RGB, 4);

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

    for (int32_t i = 0; i < _MAX_UI_LISTS_AMOUNT_; i++) ui_lists[i].active = 0;
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;

    // pages init
    for (int32_t i = 0; i < PAGES_AMOUNT; i++) {
        if (pages[i].init != NULL) {
            pages[i].init();
        }
    }

    switch_page(PAGE_MAIN_MENU);

    return 0;
}