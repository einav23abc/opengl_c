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


#include "engine_config.h"
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


#define MOUSE_BUTTONS_AMOUNT (4)

typedef struct {
    // mouse X coordinate, relative to window.
    int32_t x;
    // mouse Y coordinate, relative to window.
    int32_t y;
    /* 0 = released
     * 1 = single click
     * 2 = double click
     * etc...
     */
    uint8_t button_states[MOUSE_BUTTONS_AMOUNT];
} mouse_t;


extern const SDL_Event event;

extern uint8_t keys[SDL_NUM_SCANCODES]; // ticks since key pressed; 0 if released
extern mouse_t mouse;

extern const float TARGET_FRAME_DELAY;

extern uint8_t running;
extern uint32_t frame_start_time;
extern const uint32_t delta_time;
extern const float delta_frames;
extern const uint32_t time;
extern const float frames;

extern const SDL_GLContext context;

extern SDL_Window* window;
extern int32_t window_width;
extern int32_t window_height;
extern int32_t window_drawable_width;
extern int32_t window_drawable_height;

extern const shader_t* default_shader;

extern SDL_Surface* default_texture_surface;
extern const texture_t* default_texture;


int32_t main(int32_t argc, char** argv);
uint32_t backend_init();
uint32_t engine_init();
void engine_handle_event();
void engine_update();
/* Gets called by the function `main` for every frame.
 *
 * Calls the `render` function (if exists) after:
 *  1. The frame buffer was set to default.
 *  2. The color buffer and depth buffer have been cleared.
 *  3. The shader was set to `default_shader`.
 *  4. Every active camera's wvp_mat was updated.
 *  5. Depth test set to `less`.
 *  6. Back faces culled and face culling enabled.
 */
void engine_render();
void engine_clean();

#endif