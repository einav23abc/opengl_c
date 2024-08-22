#ifndef FONTS_H
#define FONTS_H

#include "../../engine/engine.h"
#include "nine_slices.h"

#define _MAX_TEXT_ROW_LENGTH (64)

typedef struct {
    int32_t letters_in_row;
    int32_t letters_in_col;
    int32_t letter_width;
    int32_t letter_height;
    texture_t* font_texture;
} font_t;

extern font_t letters_font;
extern font_t big_letters_font;

void fonts_init();
// returns the drawn width
float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g);
uvec2_t get_str_boxed_size(font_t* font, char* str, float row_height);
void draw_str_boxed(char* str, font_t font, nine_slice_t nslice, uint32_t left_x, uint32_t bottom_y, uint32_t padding, uint32_t row_height);

#endif