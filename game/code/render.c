#include "game.h"

void render() {
    // clear outport fbo
    use_fbo(outport_fbo);
    glClearColor(_OUTPORT_BACKGROUND_COLOR_R_, _OUTPORT_BACKGROUND_COLOR_G_, _OUTPORT_BACKGROUND_COLOR_B_, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (page >= 0 && page < PAGES_AMOUNT) {
        if (pages[page].render != NULL) {
            pages[page].render();
        }
    }

    // draw outport frame buffer to screen
    use_default_fbo();

    uint32_t pixel_scale = uintmin(window_drawable_width/_OUTPORT_WIDTH_, window_drawable_height/_OUTPORT_HEIGHT_);
    uint32_t w = _OUTPORT_WIDTH_*pixel_scale;
    uint32_t h = _OUTPORT_HEIGHT_*pixel_scale;
    if (pixel_scale < 1) {
        float fpixel_scale = fmin(((float)window_drawable_width)/_OUTPORT_WIDTH_, ((float)window_drawable_height)/_OUTPORT_HEIGHT_);
        w = _OUTPORT_WIDTH_*fpixel_scale;
        h = _OUTPORT_HEIGHT_*fpixel_scale;
    }
    glViewport((window_drawable_width-w)*0.5,(window_drawable_height-h)*0.5,w,h);
    
    simple_draw_module_draw_fbo_color_texture(outport_fbo);
}

