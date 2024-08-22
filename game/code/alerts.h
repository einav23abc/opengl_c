#ifndef ALERTS_H
#define ALERTS_H

#include "game.h"


#define _MAX_ALERTS_AMOUNT_ (8)

typedef struct {
    int32_t time_to_live;

    int32_t initial_time_to_live;
    float y_full_transform;
    easing_function_t easing_function;

    font_t* font;
    int32_t padding;

    uint8_t box_pos_from_world_pos : 1;
    float box_world_pos_x;
    float box_world_pos_y;
    float box_world_pos_z;
    camera_t* box_world_pos_camera;
    // if box_pos_from_world_pos = 1, this is a screen-x translation
    int32_t x;
    // if box_pos_from_world_pos = 1, this is a screen-y translation
    int32_t y;

    char* string;
} alert_t;

extern alert_t alerts[_MAX_ALERTS_AMOUNT_];

// will not return 0. alert 0 is error alert
int32_t new_alert_assign_id();
uvec2_t get_alert_box_size(int32_t i);
uvec2_t get_alert_box_pos(int32_t i);
uvec2_t get_alert_box_pos_padded(int32_t i);
void add_error_alert_at_cursor(char* string);
void close_all_alerts();
void draw_all_alerts();


#endif