#include "alerts.h"

alert_t alerts[_MAX_ALERTS_AMOUNT_];


int32_t new_alert_assign_id() {
    for (int32_t i = 1; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live <= 0) return i;
    }
    // get shortest time to live alert
    int32_t min_time_to_live = alerts[1].time_to_live;
    int32_t min_time_to_live_i = 1;
    for (int32_t i = 2; i < _MAX_ALERTS_AMOUNT_; i++) {
        if (alerts[i].time_to_live < min_time_to_live) {
            min_time_to_live = alerts[i].time_to_live;
            min_time_to_live_i = i;
        }
    }
    return min_time_to_live_i;
}
uvec2_t get_alert_box_size(int32_t i) {
    return get_str_boxed_size(alerts[i].string, _ALERT_ROW_HEIGHT_);
}
uvec2_t get_alert_box_pos(int32_t i) {
    float x = 0;
    float y = 0;
    if (alerts[i].box_pos_from_world_pos == 1) {
        vec2_t screen_cord = outport_space_position_from_world_space(vec3(alerts[i].box_world_pos_x, alerts[i].box_world_pos_y, alerts[i].box_world_pos_z));
        x = screen_cord.x;
        y = screen_cord.y;
    }

    x += alerts[i].x;
    y += alerts[i].y;

    if (&(alerts[i].easing_function) != NULL) {
        float t = 1-(((float)alerts[i].time_to_live) / (alerts[i].initial_time_to_live));
        y += alerts[i].easing_function(t) * alerts[i].y_full_transform;
    }

    if (x < _ALERT_PADDING_) x = _ALERT_PADDING_;
    if (y < _ALERT_PADDING_) y = _ALERT_PADDING_;

    return (uvec2_t){
        .x = x,
        .y = y
    };
}
uvec2_t get_alert_box_pos_padded(int32_t i) {
    uvec2_t box_pos = get_alert_box_pos(i);
    box_pos.x -= _ALERT_PADDING_;
    box_pos.y -= _ALERT_PADDING_;
    return box_pos;
}
void add_error_alert_at_cursor(char* string) {
    vec2_t mouse_outport_space_position = get_mouse_outport_space_position();
    
    uvec2_t string_size = get_str_boxed_size(string, _ALERT_ROW_HEIGHT_);
    
    alerts[0] = (alert_t){
        .time_to_live = 3000,

        .initial_time_to_live = 3000,
        .y_full_transform = string_size.y*3,
        .easing_function = &ease_out_sine,

        .box_pos_from_world_pos = 0,
        .x = mouse_outport_space_position.x - string_size.x*0.5,
        .y = mouse_outport_space_position.y - string_size.y*0.5,

        .string = string
    };
}
void close_all_alerts() {
    for (int32_t i = 0; i < _MAX_ALERTS_AMOUNT_; i++) alerts[i].time_to_live = 0;
}

