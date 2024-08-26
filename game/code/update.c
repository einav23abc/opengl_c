#include "game.h"
#include "ui_lists.h"
#include "alerts.h"

void update() {
    ui_lists_update();
    
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