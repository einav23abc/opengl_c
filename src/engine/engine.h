#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "engine_types.h"
#include "engine_functions.h"

// <externs>
    // <set by engine config>
        extern const uint32_t OUTPORT_WIDTH;
        extern const uint32_t OUTPORT_HEIGHT;
        extern const uint32_t OUTPORT_VIEWPORT_SIZE;
        extern const int32_t OUTPORT_VIEWPORT_X;
        extern const int32_t OUTPORT_VIEWPORT_Y;

        extern const uint32_t WINDOW_START_WIDTH;
        extern const uint32_t WINDOW_START_HEIGHT;

        const float DEFAULT_BACKGROUND_COLOR_R;
        const float DEFAULT_BACKGROUND_COLOR_G;
        const float DEFAULT_BACKGROUND_COLOR_B;

        const float DEFAULT_PILLARBOX_COLOR_R;
        const float DEFAULT_PILLARBOX_COLOR_G;
        const float DEFAULT_PILLARBOX_COLOR_B;

        extern const float TARGET_FRAME_DELAY;
    // </set by engine config>

    extern const SDL_Event event;

    extern uint8_t keys[285];

    extern uint8_t running;
    extern uint32_t frame_start_time;
    extern const uint32_t delta_time;
    extern const float delta_frames;
    extern uint32_t acum_frames_time;
    extern uint32_t acum_frames_amount;
    
    extern SDL_Window* window;
    extern int32_t window_width;
    extern int32_t window_height;
    extern fbo_t* window_fbo;

    extern const SDL_GLContext context;

    extern const fbo_t* outport_fbo;
    extern const shader_t* screen_quad_mesh_shader;
    extern const mesh_t* screen_quad_mesh;

    extern const shader_t* default_shader;

    extern const texture_t* default_texture;

    extern uint64_t shaders_amount;
    extern int64_t current_shader;
    extern shader_t* shaders_list[];

    extern uint64_t cameras_amount;
    extern int64_t current_camera;
    extern camera_t* cameras_list[];

    extern uint64_t textures_amount;
    extern texture_t* textures_list[];
    
    extern uint64_t meshes_amount;
    extern mesh_t* meshes_list[];

    extern uint64_t fbos_amount;
    extern uint64_t current_fbo;
    extern fbo_t* fbos_list[];

    
    extern const uint32_t QUAD_VAO_INDICES_ARR[];

    extern const uint64_t SHADERS_MAX_AMOUNT;
    extern const uint64_t CAMERAS_MAX_AMOUNT;
    extern const uint64_t TEXTURES_MAX_AMOUNT;
    extern const uint64_t OBJECT_TYPES_MAX_AMOUNT;
    extern const uint64_t MESHES_MAX_AMOUNT;
    extern const uint64_t FBOS_MAX_AMOUNT;

    // <simple draw module>
    extern float simple_draw_module_color_r;
    extern float simple_draw_module_color_g;
    extern float simple_draw_module_color_b;
    extern float simple_draw_module_color_a;
    extern const shader_t* simple_draw_module_cube_shader;
    extern const mesh_t* simple_draw_module_rectangle_mesh;
    extern const mesh_t* simple_draw_module_cube_mesh;
    extern const mesh_t* simple_draw_module_ball_mesh;
    // </simple draw module>
// </externs>

#endif