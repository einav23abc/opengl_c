#include "game.h"


// variable declarations

fbo_t* outport_fbo;

texture_t* global_texture;
shader_t* global_shader;

vec3_t camera_pos;
camera_t* camera;

mesh_t* cube_mesh;

game_t game_struct;

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;
shader_t* sun_shadow_map_shader;