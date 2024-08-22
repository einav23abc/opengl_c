#include "game.h"
#include "meshes.h"
#include "shaders.h"
#include "textures.h"
#include "sounds.h"
#include "nine_slices.h"
#include "fonts.h"
#include "ui_lists.h"
#include "alerts.h"

uint8_t init() {
    game_meshes_init();
    game_shaders_init();
    game_textures_init();
    game_sounds_init();
    nine_slices_init();
    fonts_init();

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