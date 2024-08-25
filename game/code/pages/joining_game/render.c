#include "page.h"
#include "../../shaders.h"
#include "../scrolled_background.h"
#include "../../nine_slices.h"
#include "../../ui_lists.h"
#include "../../alerts.h"
#include "../../fonts.h"

void render_joining_game() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    draw_scrolled_background();

    char message[] = "Joining game";
    
    ivec2_t size = get_str_boxed_size(&big_letters_font, message, big_letters_font.letter_height);
    draw_str_boxed(
        message,
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