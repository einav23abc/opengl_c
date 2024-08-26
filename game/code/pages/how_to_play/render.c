#include "page.h"
#include "../../shaders.h"
#include "../scrolled_background.h"
#include "../../nine_slices.h"
#include "../../ui_lists.h"
#include "../../alerts.h"
#include "../../fonts.h"


void render_how_to_play() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    draw_scrolled_background();

    char str[] =
        ""      "The game is played using only the mouse.\n"
        "\n"
        ""      "    Build structures in your side of\n"
        ""      "        the grid to gain weight.\n"
        ""      "The first player to be heavier by 5 wins.\n"
        "\n"
        ""      " Think strategicly about what you build!\n"
        ""      " Using strategies simmilar to those used\n"
        ""      "  in 'Settlers of catan' is beneficial.\n"
        "\n"
        ""      "     Using the 'Soldiers' resource,\n"
        ""      "  you can attack the enemy's buildings."
    ;

    ivec2_t size = get_str_boxed_size(&big_letters_font, str, big_letters_font.letter_height);
    draw_str_boxed(
        str,
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