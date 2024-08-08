#ifndef FRAME_BUFFER_OBJECTS_H
#define FRAME_BUFFER_OBJECTS_H

#include <SDL2/SDL.h>
#include "../glad/glad.h"
#include <stdint.h>
#include <stdio.h>

#include "../cameras/cameras.h"

typedef struct {
    const uint64_t fbo_index;

    uint32_t fbo_id;

    uint32_t width;
    uint32_t height;

    // none/texture/rbo
    uint32_t color;

    // 0 = no color attachment
    // 1 = texture
    // 2 = render buffer object
    uint8_t color_status : 2;
    GLint color_format;

    // none/texture/rbo
    uint32_t depth_stencil;

    // 0 = no depth and stencil attachment
    // 1 = texture
    // 2 = texture, only depth
    // 3 = texture, only stencil
    // 4 = render buffer object
    // 5 = render buffer object, only depth
    // 6 = render buffer object, only stencil
    uint8_t depth_stencil_status : 3;
} fbo_t;



extern int32_t fbos_amount;
extern int32_t current_fbo;
extern fbo_t* fbos_list[];
extern const uint32_t FBOS_MAX_AMOUNT;



/* Creates a frame-buffer-object (fbo).
 * 
 * The memory delocation of this fbo is handled by the engine (by calling `clean_fbos()`).
 *
 * \param width The width of the fbo's textures/render-objects.
 * \param height The height of the fbo's textures/render-objects.
 * \param color_status Sets how the fbo will store color data:
 * `0` - no color attachment.
 * `1` - texture.
 * `2` - render buffer object.
 * \param color_format Only relevent for texture color attachment. `GL_RGB`, `GL_RGBA`, etc. Null can be passed for no function.
 * \param color_texture_param_func Only relevent for texture color attachment.
 * A pointer to a function that will be called after the color texture is binded and before it is attached to the fbo.
 * Should be used to call `glTexParameter`. Null can be passed instead for no function.
 * \param depth_stencil_status Sets how the fbo will store depth and stencil data:
 * `0` - no depth and stencil attachment.
 * `1` - texture.
 * `2` - texture, only depth.
 * `3` - texture, only stencil.
 * `4` - render buffer object.
 * `5` - render buffer object, only depth.
 * `6` - render buffer object, only stencil.
 * \param depth_stencil_texture_param_func Only relevent for texture depth-stencil attachments. Null can be passed for no function.
 * 
 * \returns A pointer to an allocated fbo_t or `NULL` on failure.
*/
fbo_t* create_fbo_ext_param(uint32_t width, uint32_t height,
                            uint8_t color_status,
                            GLint color_format,
                            void(*color_texture_param_func)(),
                            uint8_t depth_stencil_status,
                            void(*depth_stencil_texture_param_func)());

/* Creates a frame-buffer-object (fbo).
 * 
 * The destruction of this fbo is handled by the engine (by calling `clean_fbos()`).
 *
 * \param width The width of the frame buffer object's textures/render objects.
 * \param height The height of the frame buffer object's textures/render objects.
 * \param color_status Sets how the fbo will store color data:
 * `0` - no color attachment.
 * `1` - texture.
 * `2` - render buffer object.
 * \param color_format Only relevent for texture color attachment. `GL_RGB`, `GL_RGBA`, etc. Null can be passed for no function.
 * A pointer to a function that will be called after the color texture is binded and before it is attached to the frame buffer object.
 * Should be used to call `glTexParameter`. Null can be passed instead for no function.
 * \param depth_stencil_status Sets how the fbo will store depth and stencil data:
 * `0` - no depth and stencil attachment.
 * `1` - texture.
 * `2` - texture, only depth.
 * `3` - texture, only stencil.
 * `4` - render buffer object.
 * `5` - render buffer object, only depth.
 * `6` - render buffer object, only stencil.
 * 
 * \returns A pointer to an allocated fbo_t or `NULL` on failure.
*/
fbo_t* create_fbo(  uint32_t width, uint32_t height,
                    uint8_t color_status,
                    GLint color_format,
                    uint8_t depth_stencil_status);

/* Sets the fbo that will be used for all draw functions to the defaule fbo (the window), Until a diffrent fbo is used.
 *
 * Updates the fbo's viewport by calling `camera_update_fbo_viewport()` with the `current_camera`,
*/
void use_default_fbo();

/* Sets the fbo that will be used for all draw functions, Until a diffrent fbo is used.
 *
 * Updates the fbo's viewport by calling `camera_update_fbo_viewport()` with the `current_camera`,
 * 
 * \param fbo The fbo to use.
*/
void use_fbo(fbo_t* fbo);
void bind_fbo_color_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num);
void bind_fbo_depth_stencil_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num);
// * Will not destroy the `current_fbo`. If you wish to do so, call `use_fbo()` with a diffrent fbo first.
// * \param fbo the fbo to destroy. This fbo will not be usable after destruction.
// * \returns `0` on success or `-1` on failure
int32_t destroy_fbo(fbo_t* fbo);
/* Called by the engine when exiting.
 * Destroys all created fbos.
*/
void clean_fbos();

#endif