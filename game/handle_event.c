#include "game.h"

void handle_event() {
    switch(event.type) {
        
        case SDL_MOUSEBUTTONDOWN: {
            ivec2_t pressed_on_tile = get_hovered_tile_position(0);
            
            if (selected_tile.x == -1 && selected_tile.y == -1) {
                selected_tile = pressed_on_tile;
            }else {
                selected_tile = pressed_on_tile;
            }

            break;
        }
    }
    

    return;
}