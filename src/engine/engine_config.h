#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <stdint.h>
#include <string.h>

extern char* WINDOW_TITLE;

extern uint32_t WINDOW_START_WIDTH;
extern uint32_t WINDOW_START_HEIGHT;
extern uint32_t WINDOW_START_X;
extern uint32_t WINDOW_START_Y;

extern float BACKGROUND_COLOR_R;
extern float BACKGROUND_COLOR_G;
extern float BACKGROUND_COLOR_B;

extern float TARGET_FRAME_RATE;


int32_t engine_config_init();

#endif