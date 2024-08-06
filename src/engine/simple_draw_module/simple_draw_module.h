#ifndef SIMPLE_DRAW_MODULE_H
#define SIMPLE_DRAW_MODULE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdint.h>
#include "../engine.h"

extern float simple_draw_module_color_r;
extern float simple_draw_module_color_g;
extern float simple_draw_module_color_b;
extern float simple_draw_module_color_a;
extern const shader_t* simple_draw_module_cube_shader;
extern const mesh_t* simple_draw_module_rectangle_mesh;
extern const mesh_t* simple_draw_module_cube_mesh;
extern const mesh_t* simple_draw_module_ball_mesh;

void simple_draw_module_init();

void simple_draw_module_set_color(float r, float g, float b, float a);
void simple_draw_module_draw_rect(float x, float y, float w, float h);
void simple_draw_module_draw_cube(float x, float y, float z, float w, float h, float d);
void simple_draw_module_draw_ball(float x, float y, float z, float r);
#define sdm_set_color simple_draw_module_set_color
#define sdm_draw_rect simple_draw_module_draw_rect
#define sdm_draw_cube simple_draw_module_draw_cube
#define sdm_draw_ball simple_draw_module_draw_ball

#endif