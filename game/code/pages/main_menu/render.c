#include "page.h"

void render_main_menu() {
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


    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}