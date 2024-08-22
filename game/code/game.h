#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../../engine/engine.h"

#include "meshes.h"
#include "shaders.h"
#include "textures.h"
#include "sounds.h"

#include "spaces.h"
#include "nine_slices.h"
#include "fonts.h"
#include "billboards.h"
#include "ui_lists.h"
#include "alerts.h"


#define _OUTPORT_WIDTH_ (380*2)
#define _OUTPORT_HEIGHT_ (260*2)

#define _OUTPORT_BACKGROUND_COLOR_R_ (71.0/255)
#define _OUTPORT_BACKGROUND_COLOR_G_ (65.0/255)
#define _OUTPORT_BACKGROUND_COLOR_B_ (107.0/255)

#define PAGES_AMOUNT (3)



enum PAGE_NAMES {
    PAGE_IN_GAME,
    PAGE_MAIN_MENU,
    PAGE_HOW_TO_PLAY,
};
typedef struct {
    void(*init)(void);
    void(*enter)(void);
    void(*exit)(void);
    void(*update)(void);
    void(*render)(void);
    void(*mouse_press)(void);
} page_t;

extern int32_t page;
extern page_t pages[PAGES_AMOUNT];

extern fbo_t* outport_fbo;

extern nine_slice_t nine_slice1;
extern nine_slice_t nine_slice2;
extern nine_slice_t nine_slice3;

extern font_t letters_font;
extern font_t big_letters_font;

extern camera_t* ui_camera;


void switch_page(int32_t page_i);




#endif