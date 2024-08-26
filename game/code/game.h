#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../../engine/engine.h"


#define _OUTPORT_WIDTH_ (380*2)
#define _OUTPORT_HEIGHT_ (260*2)

#define _OUTPORT_BACKGROUND_COLOR_R_ (71.0/255)
#define _OUTPORT_BACKGROUND_COLOR_G_ (65.0/255)
#define _OUTPORT_BACKGROUND_COLOR_B_ (107.0/255)

#define PAGES_AMOUNT (10)

typedef enum {
    PAGE_IN_GAME,
    PAGE_MAIN_MENU,
    PAGE_HOW_TO_PLAY,
    PAGE_SETTINGS,
    PAGE_PLAY,
    PAGE_OPEN_LAN,
    PAGE_JOIN_LAN,
    PAGE_JOINING_GAME,
    PAGE_DISCONNECTED_FROM_HOST,
    PAGE_DISCONNECTED_FROM_CLIENT
} PAGE_NAMES;
typedef struct {
    void(*init)(void);
    void(*enter)(void);
    void(*exit)(void);
    void(*update)(void);
    void(*render)(void);
    void(*mouse_press)(void);
    void(*key_press)(void);
} page_t;

extern page_t pages[PAGES_AMOUNT];
extern PAGE_NAMES page;

extern fbo_t* outport_fbo;

extern camera_t* ui_camera;


void switch_page(PAGE_NAMES page);




#endif