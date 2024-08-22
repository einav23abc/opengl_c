#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdint.h>

#include "../../engine/engine.h"

#include "meshes.h"
#include "shaders.h"
#include "textures.h"
#include "sounds.h"

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

extern vec3_t camera_pos;
extern camera_t* camera;
extern camera_t* ui_camera;

extern float sun_vector_x;
extern float sun_vector_y;
extern float sun_vector_z;
extern camera_t* sun_shadow_map_camera;
extern fbo_t* sun_shadow_map_fbo;


void switch_page(int32_t page_i);

vec2_t outport_space_position_from_world_space(fbo_t* fbo, camera_t* camera, vec3_t pos);
vec2_t get_mouse_outport_space_position(fbo_t* fbo);
vec2_t get_mouse_camera_space_position(fbo_t* fbo, camera_t* camera);
vec3_t get_mouse_world_space_position_at_y(fbo_t* fbo, camera_t* camera, float at_y);


#endif