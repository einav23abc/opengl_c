#include "game.h"

#include "pages/how_to_play/page.h"
#include "pages/in_game/page.h"
#include "pages/main_menu/page.h"

// variable declarations

page_t pages[PAGES_AMOUNT] = {
    [PAGE_IN_GAME] = (page_t){
        .init = &init_in_game,
        .enter = &enter_in_game,
        .exit = NULL,
        .update = &update_in_game,
        .render = &render_in_game,
        .mouse_press = &mouse_press_in_game
    },
    [PAGE_MAIN_MENU] = (page_t){
        .init = NULL,
        .enter = &enter_main_menu,
        .exit = NULL,
        .update = NULL,
        .render = &render_main_menu,
        .mouse_press = NULL
    },
    [PAGE_HOW_TO_PLAY] = (page_t){
        .init = NULL,
        .enter = &enter_how_to_play,
        .exit = NULL,
        .update = NULL,
        .render = &render_how_to_play,
        .mouse_press = NULL
    }
};
int32_t page = -1;

fbo_t* outport_fbo;

camera_t* ui_camera;


void switch_page(int32_t page_i) {
    if (page_i < 0 || page_i >= PAGES_AMOUNT) return;
    if (page >= 0 && page < PAGES_AMOUNT) {
        if (pages[page].exit != NULL) pages[page].exit();
    }
    if (pages[page_i].enter != NULL) pages[page_i].enter();
    page = page_i;
}

