#include "page.h"
#include "../../shaders.h"
#include "../scrolled_background.h"
#include "../../ui_lists.h"
#include "../../alerts.h"

void render_main_menu() {
    glDisable(GL_DEPTH_TEST);
    use_fbo(outport_fbo);
    use_camera(ui_camera);
    use_shader(ui_shader);

    draw_scrolled_background();

    draw_all_ui_lists();
    draw_all_alerts();

    glEnable(GL_DEPTH_TEST);
}