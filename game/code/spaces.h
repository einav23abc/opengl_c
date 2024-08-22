#ifndef SPACES_H
#define SPACES_H

#include "../../engine/engine.h"

vec2_t fbo_view_position_from_world_position(fbo_t* fbo, camera_t* camera, vec3_t pos);
vec2_t fbo_view_position_from_mouse_position(fbo_t* fbo);
vec2_t camera_view_position_from_mouse_position(fbo_t* fbo, camera_t* camera);
vec3_t world_position_at_y_from_mouse_position(fbo_t* fbo, camera_t* camera, float at_y);

#endif