#include "game.h"

void handle_event() {
    switch(event.type) {
        
        case SDL_MOUSEBUTTONDOWN: {
            // <ui-lists button clicking>
                set_ui_lists_to_unsafe();
                ivec2_t hovered_button = get_ui_list_hovered_button();
                if (hovered_button.x != -1 && hovered_button.y != -1) {
                    make_ui_list_safe(hovered_button.x);

                    audio_sound_play(button_press_sound);

                    button_callback_t button_callback = ui_lists[hovered_button.x].button_callbacks[hovered_button.y];
                    if (button_callback != NULL){
                        button_callback(hovered_button.x, hovered_button.y);
                    }

                    close_unsafe_ui_lists();
                    return;
                }
                close_unsafe_ui_lists();
            // </ui-lists button clicking>

            // page specific
            if (page >= 0 && page < PAGES_AMOUNT) {
                if (pages[page].mouse_press != NULL) {
                    pages[page].mouse_press();
                }
            }
            break;
        }
    }
    

    return;
}