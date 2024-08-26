#include "game.h"
#include "sounds.h"
#include "ui_lists.h"

void handle_event() {
    switch(event.type) {
        
        case SDL_MOUSEBUTTONDOWN: {
            // dont do page specific if inside ui-list
            ivec3_t in_ui_list = get_ui_list_inside_pos();
            
            ui_list_handle_mouse_pressed();

            // dont do page specific if inside ui-list
            if (in_ui_list.z != -1) break;

            // page specific
            if (page >= 0 && page < PAGES_AMOUNT) {
                if (pages[page].mouse_press != NULL) {
                    pages[page].mouse_press();
                }
            }
            break;
        }

        case SDL_KEYDOWN:
            // page specific
            if (page >= 0 && page < PAGES_AMOUNT) {
                if (pages[page].key_press != NULL) {
                    pages[page].key_press();
                }
            }
            break;
    }
    

    return;
}