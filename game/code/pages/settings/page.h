#ifndef PAGE_SETTINGS_H
#define PAGE_SETTINGS_H

#include "../../game.h"

typedef struct {
    float master_volume;
    float music_volume;
    float sound_volume;
} settings_t;

extern settings_t settings;

void enter_settings();
void update_settings();
void render_settings();

#endif