#include "page.h"
#include "../../shaders.h"
#include "../scrolled_background.h"
#include "../../nine_slices.h"
#include "../../ui_lists.h"
#include "../../alerts.h"
#include "../../fonts.h"

void render_join_lan() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    draw_scrolled_background();

    // ip_input_string is generated and declared in "join_lan/key_press.c"
    char ip_input_flashing_string[16];
    strcpy(ip_input_flashing_string, ip_input_string);
    // add flashing bar '|'
    if ((time%1000)/500 == 0) {
        if (ip_input_current_char < 16) {
            ip_input_flashing_string[ip_input_current_char] = '|';
        }
    }

    uvec2_t size = get_str_boxed_size(&big_letters_font, "XXX.XXX.XXX.XXX", big_letters_font.letter_height);
    draw_str_boxed(
        ip_input_flashing_string,
        big_letters_font,
        nine_slice1,
        _OUTPORT_WIDTH_*0.5  - size.x*0.5,
        _OUTPORT_HEIGHT_*0.5 - size.y*0.5,
        5,
        big_letters_font.letter_height
    );

    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}