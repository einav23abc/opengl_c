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


/* \brief Create a texture from a SDL_Surface. 
 * 
 * The memory delocation of this texture is handled by the engine (by calling `clean_textures`).
 *
 * \param texture_load_surface The SDL_Surface to create a texture from.
 * 
 * \param param_func A pointer to a function that will be called after the texture is binded and before the image is loaded.
 * Should be used to call `glTexParameter`. Null can be passed for no function.
 * 
 * \returns A pointer to an allocated texture or `NULL` on failure.
 */
texture_t* load_texture_from_surface_ext_params(SDL_Surface* texture_load_surface, void(*param_func)());

/* \brief Create a texture from a SDL_Surface.
 * 
 * The memory delocation of this texture is handled by the engine (by calling `clean_textures`).
 *
 * \param texture_load_surface The SDL_Surface to create a texture from.
 * 
 * \returns A pointer to an allocated texture or `NULL` on failure.
 */
texture_t* load_texture_from_surface(SDL_Surface* texture_load_surface);

/* \brief Create a texture from an image file. 
 * 
 * The memory delocation of this texture is handled by the engine (by calling `clean_textures`).
 *
 * \param file_path The path to the texture's image.
 * 
 * \param param_func A pointer to a function that will be called after the texture is binded and before the image is loaded.
 * Should be used to call `glTexParameter`. Null can be passed for no function.
 * 
 * \returns A pointer to an allocated texture or `NULL` on failure.
 */
texture_t* load_texture_ext_params(const char* file_path, void(*param_func)());

/* \brief Create a texture from an image file. 
 * 
 * The memory delocation of this texture is handled by the engine (by calling `clean_textures`).
 *
 * \param file_path The path to the texture's image.
 * 
 * \returns A pointer to an allocated texture or `NULL` on failure.
 */
texture_t* load_texture(const char* file_path);

/* \brief Bind a texture to a uniform for use in a shader.
 *
 * The texture will be binded to an index `texture_num` and the uniform will
 * be set to `GL_TEXTUREx` where x is the `texture_num`. Due to this, every texture
 * binded to the same shader during the same time must have a unique `texture_num`.
 * 
 * \param texture The texture to use in the shader.
 * 
 * \param uniform The location of the uniform.
 * 
 * \param texture_num The index assigned to the texture.
 */
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

/* \brief Save an SDL_Surface into a .c file.
 *
 * Can be used to load a texture without having to load from a file.
 * This can be done by calling `load_texture_from_surface` or `load_texture_from_surface_ext_params`.
 * 
 * Unlike surfaces created by SDL's functions, This surface does not need to be freed using `SDL_FreeSurface`.
 * 
 * \param image_file_path The path to the texture's image.
 * 
 * \param name The name that will be given to the surface. This will be `SDL_Surface* name`.
 * 
 * \param surface_file_path The path to the c file where the surface will be saved.
 */
void save_surface_to_c_file(const char* image_file_path, const char* name, const char* c_file_path);

#endif