#include "game.h"

#include "pages/message_page_enter.h"
#include "pages/how_to_play/page.h"
#include "pages/in_game/page.h"
#include "pages/main_menu/page.h"
#include "pages/play/page.h"
#include "pages/open_lan/page.h"
#include "pages/join_lan/page.h"
#include "pages/joining_game/page.h"
#include "pages/disconnected_from_host/page.h"
#include "pages/disconnected_from_client/page.h"

// variable declarations

page_t pages[PAGES_AMOUNT] = {
    [PAGE_IN_GAME] = (page_t){
        .init = &init_in_game,
        .enter = &enter_in_game,
        .exit = NULL,
        .update = &update_in_game,
        .render = &render_in_game,
        .mouse_press = &mouse_press_in_game,
        .key_press = NULL
    },
    [PAGE_MAIN_MENU] = (page_t){
        .init = NULL,
        .enter = &enter_main_menu,
        .exit = NULL,
        .update = NULL,
        .render = &render_main_menu,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_HOW_TO_PLAY] = (page_t){
        .init = NULL,
        .enter = &message_page_enter,
        .exit = NULL,
        .update = NULL,
        .render = &render_how_to_play,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_PLAY] = (page_t){
        .init = NULL,
        .enter = &enter_play,
        .exit = NULL,
        .update = NULL,
        .render = &render_play,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_OPEN_LAN] = (page_t){
        .init = NULL,
        .enter = &enter_open_lan,
        .exit = NULL,
        .update = NULL,
        .render = &render_open_lan,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_JOIN_LAN] = (page_t){
        .init = NULL,
        .enter = &enter_join_lan,
        .exit = NULL,
        .update = NULL,
        .render = &render_join_lan,
        .mouse_press = NULL,
        .key_press = &key_press_join_lan
    },
    [PAGE_JOINING_GAME] = (page_t){
        .init = NULL,
        .enter = &enter_joining_game,
        .exit = NULL,
        .update = NULL,
        .render = &render_joining_game,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_DISCONNECTED_FROM_HOST] = (page_t){
        .init = NULL,
        .enter = &message_page_enter,
        .exit = NULL,
        .update = NULL,
        .render = &render_disconnected_from_host,
        .mouse_press = NULL,
        .key_press = NULL
    },
    [PAGE_DISCONNECTED_FROM_CLIENT] = (page_t){
        .init = NULL,
        .enter = &message_page_enter,
        .exit = NULL,
        .update = NULL,
        .render = &render_disconnected_from_client,
        .mouse_press = NULL,
        .key_press = NULL
    }
};
PAGE_NAMES page = -1;
PAGE_NAMES next_page = -1;

fbo_t* outport_fbo;

camera_t* ui_camera;


void switch_page(PAGE_NAMES to_page) {
    if (to_page < 0 || to_page >= PAGES_AMOUNT) return;
    next_page = to_page;
}

