#include "game.h"
#include "alerts.h"

void update() {
    // switch page
    if (page != next_page) {
        if (page >= 0 && page < PAGES_AMOUNT) {
            if (pages[page].exit != NULL) pages[page].exit();
        }
        page = next_page;
        if (pages[page].enter != NULL) pages[page].enter();
    }

    if (page >= 0 && page < PAGES_AMOUNT) {
        if (pages[page].update != NULL) {
            pages[page].update();
        }
    }

    // update alerts' time to live
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live > 0) {
            alerts[i].time_to_live -= delta_time;
        }
    }
    
    return;
}