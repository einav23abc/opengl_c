#include <SDL2/SDL.h>
#include <stdint.h>
static SDL_PixelFormat nine_slice_slider_empty_texture_surface_surface_format = (SDL_PixelFormat){
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
static uint8_t nine_slice_slider_empty_texture_surface_surface_pixels[] = {
	0, 0, 0, 0, 4, 3, 3, 255, 4, 3, 3, 255, 4, 3, 3, 255, 0, 0, 0, 0, 
	4, 3, 3, 255, 28, 22, 24, 255, 28, 22, 24, 255, 28, 22, 24, 255, 4, 3, 3, 255, 
	4, 3, 3, 255, 28, 22, 24, 255, 28, 22, 24, 255, 28, 22, 24, 255, 4, 3, 3, 255, 
	4, 3, 3, 255, 28, 22, 24, 255, 28, 22, 24, 255, 28, 22, 24, 255, 4, 3, 3, 255, 
	0, 0, 0, 0, 4, 3, 3, 255, 4, 3, 3, 255, 4, 3, 3, 255, 0, 0, 0, 0
};
static SDL_Surface nine_slice_slider_empty_texture_surface_surface = {
	.flags = 8,
	.format = &nine_slice_slider_empty_texture_surface_surface_format,
	.w = 5,
	.h = 5,
	.pitch = 20,
	.pixels = &nine_slice_slider_empty_texture_surface_surface_pixels,
	.userdata = 0x0,
	.locked = 0,
	.list_blitmap = 0x0,
	.clip_rect = (SDL_Rect){
		.x = 0,
		.y = 0,
		.w = 5,
		.h = 5,
	},
	.map = 0x0,
	.refcount = 1
};
SDL_Surface* nine_slice_slider_empty_texture_surface = &nine_slice_slider_empty_texture_surface_surface;
