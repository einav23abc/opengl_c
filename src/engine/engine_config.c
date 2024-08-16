#include "engine_config.h"

// externs

extern __attribute__((weak)) char ENGINE_CONFIG_WINDOW_TITLE[];

extern __attribute__((weak)) uint32_t ENGINE_CONFIG_WINDOW_START_WIDTH;
extern __attribute__((weak)) uint32_t ENGINE_CONFIG_WINDOW_START_HEIGHT;
extern __attribute__((weak)) uint32_t ENGINE_CONFIG_WINDOW_START_X;
extern __attribute__((weak)) uint32_t ENGINE_CONFIG_WINDOW_START_Y;

extern __attribute__((weak)) float ENGINE_CONFIG_BACKGROUND_COLOR_R;
extern __attribute__((weak)) float ENGINE_CONFIG_BACKGROUND_COLOR_G;
extern __attribute__((weak)) float ENGINE_CONFIG_BACKGROUND_COLOR_B;

extern __attribute__((weak)) float ENGINE_CONFIG_TARGET_FRAME_RATE;


// declare

char* WINDOW_TITLE;

uint32_t WINDOW_START_WIDTH;
uint32_t WINDOW_START_HEIGHT;
uint32_t WINDOW_START_X;
uint32_t WINDOW_START_Y;

float BACKGROUND_COLOR_R;
float BACKGROUND_COLOR_G;
float BACKGROUND_COLOR_B;

float TARGET_FRAME_RATE;


// defaults

static char DEFAULT_WINDOW_TITLE[] = "SDL+OpenGL app";



int32_t engine_config_init() {
    if (&ENGINE_CONFIG_WINDOW_TITLE != NULL) {
        WINDOW_TITLE = ENGINE_CONFIG_WINDOW_TITLE;
    }else {
        WINDOW_TITLE = DEFAULT_WINDOW_TITLE;
    }

    if (&ENGINE_CONFIG_WINDOW_START_WIDTH != NULL) {
        WINDOW_START_WIDTH = ENGINE_CONFIG_WINDOW_START_WIDTH;
    }else {
        WINDOW_START_WIDTH = 320;
    }
    if (&ENGINE_CONFIG_WINDOW_START_HEIGHT != NULL) {
        WINDOW_START_HEIGHT = ENGINE_CONFIG_WINDOW_START_HEIGHT;
    }else {
        WINDOW_START_HEIGHT = 180;
    }
    if (&ENGINE_CONFIG_WINDOW_START_X != NULL) {
        WINDOW_START_X = ENGINE_CONFIG_WINDOW_START_X;
    }else {
        WINDOW_START_X = 100;
    }
    if (&ENGINE_CONFIG_WINDOW_START_Y != NULL) {
        WINDOW_START_Y = ENGINE_CONFIG_WINDOW_START_Y;
    }else {
        WINDOW_START_Y = 80;
    }
    
    if (&ENGINE_CONFIG_BACKGROUND_COLOR_R != NULL) {
        BACKGROUND_COLOR_R = ENGINE_CONFIG_BACKGROUND_COLOR_R;
    }else {
        BACKGROUND_COLOR_R = 0.2;
    }
    if (&ENGINE_CONFIG_BACKGROUND_COLOR_G != NULL) {
        BACKGROUND_COLOR_G = ENGINE_CONFIG_BACKGROUND_COLOR_G;
    }else {
        BACKGROUND_COLOR_G = 0.2;
    }
    if (&ENGINE_CONFIG_BACKGROUND_COLOR_B != NULL) {
        BACKGROUND_COLOR_B = ENGINE_CONFIG_BACKGROUND_COLOR_B;
    }else {
        BACKGROUND_COLOR_B = 0.2;
    }
    
    if (&ENGINE_CONFIG_TARGET_FRAME_RATE != NULL) {
        TARGET_FRAME_RATE = ENGINE_CONFIG_TARGET_FRAME_RATE;
    }else {
        TARGET_FRAME_RATE = 60;
    }

}