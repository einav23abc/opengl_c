#ifndef GAME_SOUNDS_H
#define GAME_SOUNDS_H

#include "../../engine/engine.h"

extern sound_t* build_tile_sound;
extern sound_t* attack_tile_sound;
extern sound_t* demolish_tile_sound;
extern sound_t* wheight_up_sound;
extern sound_t* wheight_down_sound;
extern sound_t* resource_give_sound;
extern sound_t* win_game_sound;
extern sound_t* lose_game_sound;
extern sound_t* switch_turn_sound;
extern sound_t* error_sound;
extern sound_t* select_tile_sound;
extern sound_t* button_press_sound;
extern music_t* talking_mud_music; // credit to https://freesound.org/people/klankbeeld/

void game_sounds_init();

#endif