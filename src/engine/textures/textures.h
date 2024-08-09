#ifndef TEXTURES_H
#define TEXTRUES_H

#include <SDL2/SDL.h>
#include "../glad/glad.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdint.h>



typedef struct {
    const uint64_t texture_index;

    uint32_t texture_id;
} texture_t;


extern uint64_t textures_amount;
extern texture_t* textures_list[];
extern const uint64_t TEXTURES_MAX_AMOUNT;


// * \param file_path The path to the texture's image
// * \param param_func A pointer to a function that will be called after the texture is binded and before the image is loaded.
// * Should be used to call `glTexParameter`. Null can be passed for no function.
texture_t* load_texture_ext_params(const char* file_path, void(*param_func)());
texture_t* load_texture(const char* file_path);
void bind_texture(texture_t* texture, int32_t uniform, uint8_t texture_num);
/* \brief Destroys a texture.
 *
 * The texture will not be usable after destruction.
 *
 * \param texture the texture to destroy.
 */
void destroy_texture(texture_t* texture);
/* Called by the engine when exiting.
 * Destroys all created textures.
 */
void clean_textures();

#endif