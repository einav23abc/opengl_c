#include "page.h"
#include "../../shaders.h"
#include "../../textures.h"
#include "../../meshes.h"
#include "../../nine_slices.h"
#include "../../ui_lists.h"
#include "../../alerts.h"


void render_how_to_play() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    // scrolled background
    // u_texture
    bind_texture(menu_background_scroll_texture, shaders_list[current_shader]->u_texture_loc, 0);
    int32_t d = ((int32_t)(time*0.01) % 128);
    for (int32_t x = -1; x < (_OUTPORT_WIDTH_/128.0)+1; x++) {
        for (int32_t y = -1; y < (_OUTPORT_HEIGHT_/128.0)+1; y++) {
            
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                (float)(x*128 + d),
                (float)(y*128 - d)
            );
            // u_scale
            glUniform2f(shaders_list[current_shader]->uniform_locations[1], 128, 128);
            draw_mesh(rect_plane_mesh);
        }
    }

    char str[] =
        ""      "The game is played using only the mouse.\n"
        "\n"
        ""      "    Build structures in your side of\n"
        ""      "        the grid to gain wheight.\n"
        ""      "The first player to be heavier by 5 wins.\n"
        "\n"
        ""      " Think strategicly about what you build!\n"
        ""      " Using strategies simmilar to those used\n"
        ""      "  in 'Settlers of catan' is beneficial.\n"
        "\n"
        ""      "     Using the 'Soldiers' resource,\n"
        ""      "  you can attack the enemy's buildings."
    ;
    uvec2_t size = get_str_boxed_size(&big_letters_font, str, big_letters_font.letter_height);
    draw_str_boxed(
        str,
        big_letters_font,
        nine_slice1,
        _OUTPORT_WIDTH_*0.5  - size.x*0.5 - 12,
        _OUTPORT_HEIGHT_*0.5 - size.y*0.5 - 12,
        12,
        big_letters_font.letter_height
    );


    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}