#include "page.h"
#include "../../shaders.h"
#include "../scrolled_background.h"
#include "../../nine_slices.h"
#include "../../ui_lists.h"
#include "../../alerts.h"
#include "../../fonts.h"

void render_open_lan() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    draw_scrolled_background();

    // open_lan_waiting_for_connection_string is generated and declared in "open_lan/enter.c"
    uvec2_t size = get_str_boxed_size(&big_letters_font, open_lan_waiting_for_connection_string, big_letters_font.letter_height);
    draw_str_boxed(
        open_lan_waiting_for_connection_string,
        big_letters_font,
        nine_slice1,
        _OUTPORT_WIDTH_*0.5  - size.x*0.5,
        _OUTPORT_HEIGHT_*0.5 - size.y*0.5,
        12,
        big_letters_font.letter_height
    );

    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}