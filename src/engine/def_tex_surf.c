#include <SDL2/SDL.h>
#include <stdint.h>
static SDL_PixelFormat surface_format = (SDL_PixelFormat){
        .format = 376840196,
        .palette = 0x0,
        .BitsPerPixel = 32,
        .BytesPerPixel = 4,
        .padding = {0, 0},
        .Rmask = 255,
        .Gmask = 65280,
        .Bmask = 16711680,
        .Amask = 4278190080,
        .Rloss = 0,
        .Gloss = 0,
        .Bloss = 0,
        .Aloss = 0,
        .Rshift = 0,
        .Gshift = 8,
        .Bshift = 16,
        .Ashift = 24,
        .refcount = 1,
        .next = 0x0
};
static uint8_t surface_pixels[] = {
        255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255
};
SDL_Surface surface = (SDL_Surface){
        .flags = 8,
        .format = &surface_format,
        .w = 2,
        .h = 2,
        .pitch = 8,
        .pixels = &surface_pixels,
        .userdata = 0x0,
        .locked = 0,
        .list_blitmap = 0x0,
        .clip_rect = (SDL_Rect){
                .x = 0,
                .y = 0,
                .w = 2,
                .h = 2,
        },
        .map = 0x0,
        .refcount = 1
};