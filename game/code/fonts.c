#include "fonts.h"

#include "textures.h"
#include "meshes.h"
#include "shaders.h"

font_t letters_font;
font_t big_letters_font;

void fonts_init() {
    // <letters_font>
        letters_font = (font_t){
            .font_texture = letters_font_texture,
            .letters_in_row = 16,
            .letters_in_col = 10,
            .letter_width = 6,
            .letter_height = 12
        };
    // </letters_font>

    // <big_letters_font>
        big_letters_font = (font_t){
            .font_texture = big_letters_font_texture,
            .letters_in_row = 16,
            .letters_in_col = 10,
            .letter_width = 12,
            .letter_height = 24
        };
    // </big_letters_font>
}
float draw_string(font_t font, char* str, vec3_t pos, quat_t rot, float height, float color_r, float color_b, float color_g) {
    int32_t string_len = min(strlen(str), _MAX_TEXT_ROW_LENGTH);

    shader_t* last_shader = shaders_list[current_shader];
    use_shader(font_shader);

    // u_texture
    bind_texture(font.font_texture, shaders_list[current_shader]->u_texture_loc, 0);
    // u_position
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[0],
        pos.x,
        pos.y,
        pos.z
    );
    // u_scale
    float size_x = string_len*font.letter_width*(height/font.letter_height);
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[1],
        size_x,
        height,
        0
    );
    // u_quat_rotation
    glUniform4f(
        shaders_list[current_shader]->uniform_locations[2],
        rot.x,
        rot.y,
        rot.z,
        rot.w
    );
    // u_text_row_length
    glUniform1i(
        shaders_list[current_shader]->uniform_locations[3],
        string_len
    );
    // u_text_row
    int32_t text_row[string_len];
    for (int32_t i = 0; i < string_len; i++) {
        text_row[i] = str[i];
    }
    glUniform1iv(
        shaders_list[current_shader]->uniform_locations[4],
        string_len,
        text_row
    );
    // u_font_data
    glUniform2i(
        shaders_list[current_shader]->uniform_locations[5],
        font.letters_in_row,
        font.letters_in_col
    );
    // u_color
    glUniform3f(
        shaders_list[current_shader]->uniform_locations[6],
        color_r,
        color_g,
        color_b
    );
    draw_mesh(rect_plane_mesh);

    use_shader(last_shader);

    return size_x;
}
ivec2_t get_str_boxed_size(font_t* font, char* str, float row_height) {
    uint32_t w = 0;
    uint32_t h = row_height;
    uint32_t x = 0;

    char ch;

    int32_t in_color_swap = 0;

    int32_t c = -1;
    while (1) {
        c += 1;

        ch = str[c];

        if (in_color_swap > 0) {
            in_color_swap -= 1;
            continue;
        }

        if (ch == '\0') {
            break;
        }
        
        if (ch == '\x1f') {
            in_color_swap = 3;
            continue;
        }
        if (ch == '\n') {
            x = 0;
            h += row_height;
            continue;
        }

        x += 1;
        w = max(w, x);
    }

    return (ivec2_t){
        .x = w * font->letter_width*(row_height/font->letter_height),
        .y = h
    };
}
void draw_str_boxed(char* str, font_t font, nine_slice_t nslice, uint32_t left_x, uint32_t bottom_y, uint32_t padding, uint32_t row_height) {
    ivec2_t str_size = get_str_boxed_size(&font, str, row_height);

    uint8_t color_r = 249;
    uint8_t color_g = 245;
    uint8_t color_b = 239;
    
    uint32_t top_y = bottom_y + str_size.y;

    uint32_t x = 0;
    uint32_t y = -row_height;

    draw_nine_slice(
        nslice,
        left_x - padding,
        top_y - str_size.y - padding,
        2*padding + str_size.x,
        2*padding + str_size.y
    );

    int32_t in_color_swap = 0;
    
    char one_char_str[2] = "X\0";

    int32_t c = -1;
    while (1) {
        c += 1;

        one_char_str[0] = str[c];

        if (in_color_swap == 3) {
            color_r = one_char_str[0];
            in_color_swap -= 1;
            continue;
        }else if (in_color_swap == 2) {
            color_g = one_char_str[0];
            in_color_swap -= 1;
            continue;
        }else if (in_color_swap == 1) {
            color_b = one_char_str[0];
            in_color_swap -= 1;
            continue;
        }

        if (one_char_str[0] == '\0') {
            break;
        }
        
        if (one_char_str[0] == '\x1f') {
            in_color_swap = 3;
            continue;
        }
        if (one_char_str[0] == '\n') {
            x = 0;
            y -= row_height;
            continue;
        }

        if (x >= str_size.x) {
            x = 0;
            y -= row_height;
        }

        x += draw_string(
            font,
            one_char_str,
            (vec3_t){
                .x = left_x + x,
                .y = top_y + y,
                .z = 0
            },
            quat_from_axis_angles_yzx(-0, -0, -0),
            row_height,
            ((float)color_r)/255.0, ((float)color_g)/255.0, ((float)color_b)/255.0
        );
    }
}