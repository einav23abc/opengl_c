#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


#include "threads/threads.h"
#include "audio/audio.h"
#include "utils/utils.h"
#include "vec_mat_quat/vec_mat_quat.h"
#include "cameras/cameras.h"
#include "shaders/shaders.h"
#include "textures/textures.h"
#include "frame_buffer_objects/frame_buffer_objects.h"
#include "meshes_and_animations/meshes_and_animations.h"
#include "simple_draw_module/simple_draw_module.h"

#include "../game/engine_config.h"


// <engine configurables>
#ifndef _WINDOW_TITLE_
#define _WINDOW_TITLE_ "SDL+OpenGL app"
#endif

#ifndef _WINDOW_START_WIDTH_
#define  _WINDOW_START_WIDTH_ (320)
#endif
#ifndef _WINDOW_START_HEIGHT_
#define  _WINDOW_START_HEIGHT_ (180)
#endif
#ifndef _WINDOW_START_X_
#define  _WINDOW_START_X_ (100)
#endif
#ifndef _WINDOW_START_Y_
#define  _WINDOW_START_Y_ (100)
#endif


#ifndef _BACKGROUND_COLOR_R_
#define  _BACKGROUND_COLOR_R_ (0.2)
#endif
#ifndef _BACKGROUND_COLOR_G_
#define  _BACKGROUND_COLOR_G_ (0.2)
#endif
#ifndef _BACKGROUND_COLOR_B_
#define  _BACKGROUND_COLOR_B_ (0.2)
#endif


#ifndef _TARGET_FRAME_RATE_
#define  _TARGET_FRAME_RATE_ (60)
#endif
#define _TARGET_FRAME_DELAY_ (1000/_TARGET_FRAME_RATE_)

// </engine configurables>


extern const SDL_Event event;

extern uint8_t keys[SDL_NUM_SCANCODES]; // ticks since key pressed; 0 if released


extern const float TARGET_FRAME_DELAY;

extern uint8_t running;
extern uint32_t frame_start_time;
extern const uint32_t delta_time;
extern const float delta_frames;

extern const SDL_GLContext context;

extern SDL_Window* window;
extern int32_t window_width;
extern int32_t window_height;
extern int32_t window_drawable_width;
extern int32_t window_drawable_height;

extern const shader_t* default_shader;
extern const texture_t* default_texture;

uint32_t backend_init();
uint32_t engine_init();
void engine_handle_event();
void engine_update();
/*
 * Gets called by the function {main} for every frame
 *
 * Calls the {render} function (if exists) after:
 *  1. The frame buffer was set to {outport_fbo}
 *  2. The color buffer and depth buffer have been cleared
 *  3. The shader was set to {default_shader}
 *  4. Every active camera's wvp_mat was updated
 *  5. Depth test set to {less}
 *  6. Back faces culled and face culling enabled
 * 
 * After the {render} function was called:
 *  1. The {outport_fbo} frame buffer will be drawn to the screen
 *  2. The {current_camera} will be set to -1
*/
void engine_render();
void engine_clean();

#endif