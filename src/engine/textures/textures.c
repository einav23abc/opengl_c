#include "textures.h"


#ifndef _TEXTURES_MAX_AMOUNT_
#define _TEXTURES_MAX_AMOUNT_ (128)
#endif
uint64_t textures_amount = 0;
texture_t* textures_list[_TEXTURES_MAX_AMOUNT_];
const uint64_t TEXTURES_MAX_AMOUNT = _TEXTURES_MAX_AMOUNT_;


texture_t* load_texture_ext_params(const char* file_path, void(*param_func)()) {
    if (textures_amount >= TEXTURES_MAX_AMOUNT) return NULL;

    texture_t* texture = malloc(sizeof(texture_t));
    if (texture == NULL) return NULL;

    uint32_t texture_id;
    SDL_Surface* texture_load_surface;
    texture_load_surface = IMG_Load(file_path);
    if (texture_load_surface == NULL){
        printf("failed to load texture '%s'. error: \"%s\"\n", file_path, SDL_GetError());
        free(texture);
        return NULL;
    }

    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (param_func != NULL) param_func();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_load_surface->w, texture_load_surface->h, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, texture_load_surface->pixels);
    
    texture->texture_id = texture_id;
    
    *((uint64_t*)&texture->texture_index) = textures_amount;
    textures_list[textures_amount] = texture;
    textures_amount += 1;
    
    SDL_FreeSurface(texture_load_surface);

    return texture;
}

texture_t* load_texture(const char* file_path) {
    return load_texture_ext_params(file_path, NULL);
}

void bind_texture(texture_t* texture, int32_t uniform, uint8_t texture_num) {
    glUniform1i(uniform, texture_num);
    glActiveTexture(GL_TEXTURE0+texture_num);
    glBindTexture(GL_TEXTURE_2D, texture->texture_id);
}

static void clean_texture(texture_t* texture) {
    glDeleteTextures(1, &texture->texture_id);
    free(texture);
}

void destroy_texture(texture_t* texture) {
    textures_amount -= 1;

    // move the last texture at textures_list to texture's index
    texture_t* last_textures = textures_list[textures_amount];
    *((uint64_t*)&last_textures->texture_index) = texture->texture_index;
    textures_list[texture->texture_index] = last_textures;

    clean_texture(texture);
}

void clean_textures() {
    printf("cleaning %u textures\n", textures_amount);
    for (uint64_t i = 0; i < textures_amount; i++) clean_texture(textures_list[i]);
    textures_amount = 0;
}