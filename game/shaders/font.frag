#version 150

#define MAX_TEXT_ROW_LENGTH (64)

varying vec2 v_texcoord;

uniform sampler2D u_texture;

uniform int u_text_row_length;
uniform int u_text_row[MAX_TEXT_ROW_LENGTH];
/* x = letters in row
 * y = letters in col
 */
uniform ivec2 u_font_data;

uniform vec3 u_color;


void main(){
    int text_row_index = int(v_texcoord.x*u_text_row_length);
    if (text_row_index >= MAX_TEXT_ROW_LENGTH) {
        text_row_index = 0;
    }
    int letter_index = u_text_row[text_row_index];

    vec2 letter_texcoord = vec2(
        v_texcoord.x*u_text_row_length - floor(v_texcoord.x*u_text_row_length),
        v_texcoord.y
    );

    if (letter_index >= u_font_data.x*u_font_data.y) {
        letter_index = 0;
    }

    vec2 font_texture_letter_position = vec2(
        float(letter_index % u_font_data.x)/float(u_font_data.x),
        float(letter_index / u_font_data.x)/float(u_font_data.y)
    );

    vec2 final_texcoord = font_texture_letter_position + letter_texcoord/vec2(u_font_data.x, u_font_data.y);

    vec4 color = texture2D(u_texture, final_texcoord);
    
    gl_FragColor = vec4(color.rgb*u_color, color.a);
}