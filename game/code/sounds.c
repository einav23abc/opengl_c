#include "sounds.h"


// #ifdef DEBUG_SOFT_MODE
// sound_t* build_tile_sound;
// sound_t* attack_tile_sound;
// sound_t* demolish_tile_sound;
// sound_t* wheight_up_sound;
// sound_t* wheight_down_sound;
// sound_t* resource_give_sound;
// sound_t* win_game_sound;
// sound_t* lose_game_sound;
// sound_t* switch_turn_sound;
// sound_t* error_sound;
// sound_t* select_tile_sound;
// sound_t* button_press_sound;
// #else
#include "../sounds/build_tile_sound.c"
#include "../sounds/attack_tile_sound.c"
#include "../sounds/demolish_tile_sound.c"
#include "../sounds/wheight_up_sound.c"
#include "../sounds/wheight_down_sound.c"
#include "../sounds/resource_give_sound.c"
#include "../sounds/win_game_sound.c"
#include "../sounds/lose_game_sound.c"
#include "../sounds/switch_turn_sound.c"
#include "../sounds/error_sound.c"
#include "../sounds/select_tile_sound.c"
#include "../sounds/button_press_sound.c"
// #endif
music_t* talking_mud_music;

void game_sounds_init() {
    // #ifdef DEBUG_SOFT_MODE
    // build_tile_sound = audio_sound_load("./game/sounds/place_tile.wav");        // \x   // X
    // attack_tile_sound = audio_sound_load("./game/sounds/attack_tile.wav");      // X    // X
    // demolish_tile_sound = audio_sound_load("./game/sounds/demolish_tile.wav");  // X    // X
    // wheight_up_sound = audio_sound_load("./game/sounds/wheight_up.wav");        // X    // X
    // wheight_down_sound = audio_sound_load("./game/sounds/wheight_down.wav");    // X    // X
    // resource_give_sound = audio_sound_load("./game/sounds/resource_give.wav");  // X    // X
    // win_game_sound = audio_sound_load("./game/sounds/win_game.wav");            // /x   // X
    // lose_game_sound = audio_sound_load("./game/sounds/lose_game.wav");          // /x   // X
    // switch_turn_sound = audio_sound_load("./game/sounds/switch_turn.wav");      // X    // X
    // button_press_sound = audio_sound_load("./game/sounds/button_press.wav");    // X    // X
    // error_sound = audio_sound_load("./game/sounds/error.wav");                  // X    // X
    // select_tile_sound = audio_sound_load("./game/sounds/select_tile.wav");      // X    // X
    // #endif

    // audio_save_sound_to_c_file(build_tile_sound, "build_tile_sound", "./game/sounds/build_tile_sound.c");
    // audio_save_sound_to_c_file(attack_tile_sound, "attack_tile_sound", "./game/sounds/attack_tile_sound.c");
    // audio_save_sound_to_c_file(demolish_tile_sound, "demolish_tile_sound", "./game/sounds/demolish_tile_sound.c");
    // audio_save_sound_to_c_file(wheight_up_sound, "wheight_up_sound", "./game/sounds/wheight_up_sound.c");
    // audio_save_sound_to_c_file(wheight_down_sound, "wheight_down_sound", "./game/sounds/wheight_down_sound.c");
    // audio_save_sound_to_c_file(resource_give_sound, "resource_give_sound", "./game/sounds/resource_give_sound.c");
    // audio_save_sound_to_c_file(win_game_sound, "win_game_sound", "./game/sounds/win_game_sound.c");
    // audio_save_sound_to_c_file(lose_game_sound, "lose_game_sound", "./game/sounds/lose_game_sound.c");
    // audio_save_sound_to_c_file(switch_turn_sound, "switch_turn_sound", "./game/sounds/switch_turn_sound.c");
    // audio_save_sound_to_c_file(button_press_sound, "button_press_sound", "./game/sounds/button_press_sound.c");
    // audio_save_sound_to_c_file(error_sound, "error_sound", "./game/sounds/error_sound.c");
    // audio_save_sound_to_c_file(select_tile_sound, "select_tile_sound", "./game/sounds/select_tile_sound.c");

    talking_mud_music = audio_music_load("./game/sounds/talking_mud.flac");
    if (talking_mud_music == NULL) {
        talking_mud_music = audio_music_load("./src/sounds/talking_mud.flac");
    }
    if (talking_mud_music != NULL) {
        audio_music_play(talking_mud_music);
    }
}