#ifndef SIMPLE_DRAW_MODULE_H
#define SIMPLE_DRAW_MODULE_H

#include <SDL2/SDL.h>
#include "../glad/glad.h"
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
extern const shader_t* simple_draw_module_screen_quad_mesh_shader;
extern const mesh_t* simple_draw_module_screen_quad_mesh;

void simple_draw_module_init();

void simple_draw_module_set_color(float r, float g, float b, float a);
void simple_draw_module_draw_rect(float x, float y, float w, float h);
void simple_draw_module_draw_cube(float x, float y, float z, float w, float h, float d);
void simple_draw_module_draw_ball(float x, float y, float z, float r);
/* \brief Draws a texture to the entire viewport on the current fbo.
 *
 * The texture will stretch to fill the entire viewport.
 * 
 * \param texture The texture to draw.
*/
void simple_draw_module_draw_texture(texture_t* texture);
/* \brief Draws an fbo's color texture to the entire viewport on the current fbo.
 *
 * The texture will stretch to fill the entire viewport.
 * 
 * \param texture The texture to draw.
*/
void simple_draw_module_draw_fbo_color_texture(fbo_t* fbo);
/* \brief Draws an fbo's depth-stencil texture to the entire viewport on the current fbo.
 *
 * The texture will stretch to fill the entire viewport.
 * 
 * \param texture The texture to draw.
*/
void simple_draw_module_draw_fbo_depth_stencil_texture(fbo_t* fbo);
#define sdm_set_color simple_draw_module_set_color
#define sdm_draw_rect simple_draw_module_draw_rect
#define sdm_draw_cube simple_draw_module_draw_cube
#define sdm_draw_ball simple_draw_module_draw_ball
#define sdm_draw_texture simple_draw_module_draw_texture
#define sdm_draw_fbo_color_texture simple_draw_module_draw_fbo_color_texture
#define sdm_draw_fbo_depth_stencil_texture simple_draw_module_draw_fbo_depth_stencil_texture

#endif