#include "textures.h"


#ifndef _TEXTURES_MAX_AMOUNT_
#define _TEXTURES_MAX_AMOUNT_ (128)
#endif
uint64_t textures_amount = 0;
texture_t* textures_list[_TEXTURES_MAX_AMOUNT_];
const uint64_t TEXTURES_MAX_AMOUNT = _TEXTURES_MAX_AMOUNT_;


texture_t* load_texture_from_surface_ext_params(SDL_Surface* texture_load_surface, void(*param_func)()) {
    if (textures_amount >= TEXTURES_MAX_AMOUNT) return NULL;
    
    texture_t* texture = malloc(sizeof(texture_t));
    if (texture == NULL) return NULL;

    uint32_t texture_id;

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

    return texture;
}

texture_t* load_texture_from_surface(SDL_Surface* texture_load_surface) {
    return load_texture_from_surface_ext_params(texture_load_surface, NULL);
}

texture_t* load_texture_ext_params(const char* file_path, void(*param_func)()) {
    SDL_Surface* texture_load_surface;
    texture_load_surface = IMG_Load(file_path);
    if (texture_load_surface == NULL){
        printf("failed to load image '%s'. error: \"%s\"\n", file_path, SDL_GetError());
        return NULL;
    }

    texture_t* texture =  load_texture_from_surface_ext_params(texture_load_surface, param_func);

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
    #ifdef DEBUG_SOFT_MODE
    printf("cleaning %u textures\n", textures_amount);
    #endif
    for (uint64_t i = 0; i < textures_amount; i++) clean_texture(textures_list[i]);
    textures_amount = 0;
}

void save_surface_to_c_file(const char* image_file_path, const char* name, const char* c_file_path) {
    FILE* fp;
    fp = fopen(c_file_path, "w");
    if (fp == NULL) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open file \"%s\" to save surface \"%s\" to.\n", c_file_path, name);
        #endif
        return;
    }
    
    SDL_Surface* surface;
    surface = IMG_Load(image_file_path);
    if (surface == NULL){
        printf("failed to load image '%s'. error: \"%s\"\n", image_file_path, SDL_GetError());
        fclose(fp);
        return;
    }

    printf("\nprinting surface for saving:\n");
    if (surface->format->palette != NULL) printf("ATTENTION! format->palette isnt NULL. Might not work when saved.\n");
    if (surface->format->next != NULL)    printf("ATTENTION! format has next.           Might not work when saved.\n");
    if (surface->userdata != NULL)        printf("ATTENTION! userdata isnt NULL.        Might not work when saved.\n");
    if (surface->list_blitmap != NULL)    printf("ATTENTION! list_blitmap isnt NULL.    Might not work when saved.\n");
    if (surface->map != NULL)             printf("ATTENTION! map isnt NULL.             Might not work when saved.\n");
    printf("\n");

    fprintf(fp,
        ""      "#include <SDL2/SDL.h>\n"
        ""      "#include <stdint.h>\n"
    );

    // surface_format
    fprintf(fp,
        ""      "static SDL_PixelFormat %s_surface_format = (SDL_PixelFormat){\n"
        "\t"        ".format = %u,\n"
        "\t"        ".palette = 0x0,\n"
        "\t"        ".BitsPerPixel = %hhu,\n"
        "\t"        ".BytesPerPixel = %hhu,\n"
        "\t"        ".padding = {%hhu, %hhu},\n"
        "\t"        ".Rmask = %u,\n"
        "\t"        ".Gmask = %u,\n"
        "\t"        ".Bmask = %u,\n"
        "\t"        ".Amask = %u,\n"
        "\t"        ".Rloss = %hhu,\n"
        "\t"        ".Gloss = %hhu,\n"
        "\t"        ".Bloss = %hhu,\n"
        "\t"        ".Aloss = %hhu,\n"
        "\t"        ".Rshift = %hhu,\n"
        "\t"        ".Gshift = %hhu,\n"
        "\t"        ".Bshift = %hhu,\n"
        "\t"        ".Ashift = %hhu,\n"
        "\t"        ".refcount = %d,\n"
        "\t"        ".next = 0x0\n"
        ""      "};\n"
        ,
        name,
        surface->format->format,
        surface->format->BitsPerPixel,
        surface->format->BytesPerPixel,
        surface->format->padding[0], surface->format->padding[1],
        surface->format->Rmask,
        surface->format->Gmask,
        surface->format->Bmask,
        surface->format->Amask,
        surface->format->Rloss,
        surface->format->Gloss,
        surface->format->Bloss,
        surface->format->Aloss,
        surface->format->Rshift,
        surface->format->Gshift,
        surface->format->Bshift,
        surface->format->Ashift,
        surface->format->refcount
    );

    // surface_pixels
    fprintf(fp,
        ""      "static uint8_t %s_surface_pixels[] = {"
        ,
        name
    );
    for (uint64_t i = 0; i < surface->w*surface->h*surface->format->BytesPerPixel; i++) {
        if (i != 0) fprintf(fp,", ");
        if (i%20 == 0) fprintf(fp,"\n\t");
        fprintf(fp,"%hhu", ((uint8_t*)(surface->pixels))[i]);
    }
    fprintf(fp,"\n};\n");

    // surface
    fprintf(fp,
        ""      "static SDL_Surface %s_surface = {\n"
        "\t"        ".flags = %u,\n"
        "\t"        ".format = &%s_surface_format,\n"
        "\t"        ".w = %d,\n"
        "\t"        ".h = %d,\n"
        "\t"        ".pitch = %d,\n"
        "\t"        ".pixels = &%s_surface_pixels,\n"
        "\t"        ".userdata = 0x0,\n"
        "\t"        ".locked = %d,\n"
        "\t"        ".list_blitmap = 0x0,\n"
        "\t"        ".clip_rect = (SDL_Rect){\n"
        "\t\t"          ".x = %d,\n"
        "\t\t"          ".y = %d,\n"
        "\t\t"          ".w = %d,\n"
        "\t\t"          ".h = %d,\n"
        "\t"        "},\n"
        "\t"        ".map = 0x0,\n"
        "\t"        ".refcount = %d\n"
        ""      "};\n"
        ,
        name,
        surface->flags,
        name,
        surface->w,
        surface->h,
        surface->pitch,
        name,
        surface->locked,
        surface->clip_rect.x,
        surface->clip_rect.y,
        surface->clip_rect.w,
        surface->clip_rect.h,
        surface->refcount
    );

    // name
    fprintf(fp,
        ""      "SDL_Surface* %s = &%s_surface;\n"
        ,
        name,
        name
    );

    SDL_FreeSurface(surface);
    fclose(fp);
}