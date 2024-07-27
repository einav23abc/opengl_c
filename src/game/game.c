#include "game_types.h"
#include "game_functions.h"

// <engine config>
const uint32_t ENGINE_CONFIG_OUTPORT_WIDTH = 320;
const uint32_t ENGINE_CONFIG_OUTPORT_HEIGHT = 240;

const uint32_t ENGINE_CONFIG_WINDOW_START_WIDTH = 320*3;
const uint32_t ENGINE_CONFIG_WINDOW_START_HEIGHT = 240*3;

const float ENGINE_CONFIG_TARGET_FRAME_DELAY = (1000/60);

const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_R = 0.2;
const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_G = 0.2;
const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_B = 0.3;

const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_R = 0.1;
const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_G = 0.1;
const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_B = 0.1;


// variable declarations
texture_t* global_texture;
shader_t* global_shader;

camera_t* player_camera;
mesh_t* player_mesh;
player_t player;

mesh_t* man_mesh;

mesh_t* cube_mesh;
#define _CUBES_AMOUNT_ (3)
cube_t cubes[_CUBES_AMOUNT_];

float sun_vector_x;
float sun_vector_y;
float sun_vector_z;
camera_t* sun_shadow_map_camera;
fbo_t* sun_shadow_map_fbo;
shader_t* sun_shadow_map_shader;

// constants
const uint64_t CUBES_AMOUNT = _CUBES_AMOUNT_;

// functions
void cube_update_aabb(cube_t* cube) {
    mat_t rotation_x_matrix;
    mat_t rotation_y_matrix;
    mat_t rotation_z_matrix;
    mat_t rotation_matrix = matrix_create((uvec2_t){3,3});
    mat_t ping_pong_matrix1 = matrix_create((uvec2_t){3,3});
    mat_t vert;
    mat_t result = matrix_create((uvec2_t){1,3});

    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
    uint8_t i;
    
    rotation_x_matrix = (mat_t){
        .size = (uvec2_t){
            .x = 3,
            .y = 3
        },
        .mat = (float[9]){
            1, 0,               0,
            0, cos(-cube->rx), -sin(-cube->rx),
            0, sin(-cube->rx),  cos(-cube->rx),
        }
    };
    rotation_y_matrix = (mat_t){
        .size = (uvec2_t){
            .x = 3,
            .y = 3
        },
        .mat = (float[9]){
             cos(-cube->ry), 0, sin(-cube->ry),
             0,              1, 0,
            -sin(-cube->ry), 0, cos(-cube->ry),
        }
    };
    rotation_z_matrix = (mat_t){
        .size = (uvec2_t){
            .x = 3,
            .y = 3
        },
        .mat = (float[9]){
            cos(-cube->rz), -sin(-cube->rz), 0,
            sin(-cube->rz),  cos(-cube->rz), 0,
            0,               0,              1,
        }
    };
    matrix_multiply(&rotation_z_matrix, &rotation_y_matrix, &ping_pong_matrix1);
    matrix_multiply(&rotation_x_matrix, &ping_pong_matrix1, &rotation_matrix);

    for (i = 0; i < 8; i++) {
        vert = (mat_t){
            .size = (uvec2_t){
                .x = 1,
                .y = 3
            },
            .mat = (float[3]){
                0.5*cube->w*(((i&0b001)==0)*2.0-1.0),
                0.5*cube->h*(((i&0b010)==0)*2.0-1.0),
                0.5*cube->d*(((i&0b100)==0)*2.0-1.0)
            }
        };
        matrix_multiply(&rotation_matrix, &vert, &result);

        cube->vertices[i] = (vec3_t){
            .x = result.mat[0]+cube->x,
            .y = result.mat[1]+cube->y,
            .z = result.mat[2]+cube->z
        };

        if (i == 0) {
            min_x = cube->vertices[0].x;
            max_x = cube->vertices[0].x;
            min_y = cube->vertices[0].y;
            max_y = cube->vertices[0].y;
            min_z = cube->vertices[0].z;
            max_z = cube->vertices[0].z;
        }else {
            min_x = fmin(min_x, cube->vertices[i].x);
            max_x = fmax(max_x, cube->vertices[i].x);
            min_y = fmin(min_y, cube->vertices[i].y);
            max_y = fmax(max_y, cube->vertices[i].y);
            min_z = fmin(min_z, cube->vertices[i].z);
            max_z = fmax(max_z, cube->vertices[i].z);
        }
    }

    cube->aabb_x = min_x;
    cube->aabb_y = min_y;
    cube->aabb_z = min_z;
    cube->aabb_w = max_x-min_x;
    cube->aabb_h = max_y-min_y;
    cube->aabb_d = max_z-min_z;

    free(rotation_matrix.mat);
    free(ping_pong_matrix1.mat);
    free(result.mat);

    return;
}

void cube_draw(cube_t* cube) {
    // u_position
    glUniform3f(shaders_list[current_shader]->uniform_locations[0], cube->x, cube->y, cube->z);
    // u_scale
    glUniform3f(shaders_list[current_shader]->uniform_locations[1], cube->w, cube->h, cube->d);
    // u_rotation
    glUniform3f(shaders_list[current_shader]->uniform_locations[2], -cube->rx, -cube->ry, -cube->rz);
    draw_with_mesh(cube_mesh);
}

void cube_debug_draw_vertices(cube_t* cube) {
    sdm_set_color(1, 0, 0, 1);
    for (uint8_t j = 0; j < 8; j++) {
        sdm_draw_ball(cube->vertices[j].x, cube->vertices[j].y, cube->vertices[j].z, 5);
    }
}

vec3_t inline vec3_cross_product(vec3_t vec1, vec3_t vec2) {
    return (vec3_t){
        .x = vec1.y*vec2.z - vec1.z*vec2.y,
        .y = vec1.z*vec2.x - vec1.x*vec2.z,
        .z = vec1.x*vec2.y - vec1.y*vec2.x
    };
}

// returns the collision resolution vector for cube1
// or (0,0,0) vector if no collision
vec3_t sat_cube_collision(cube_t* cube1, cube_t* cube2) {
    // check AABB
    if (
        cube1->aabb_x               > cube2->aabb_x+cube2->aabb_w ||
        cube1->aabb_x+cube1->aabb_w < cube2->aabb_x               ||
        cube1->aabb_y               > cube2->aabb_y+cube2->aabb_h ||
        cube1->aabb_y+cube1->aabb_h < cube2->aabb_y               ||
        cube1->aabb_z               > cube2->aabb_z+cube2->aabb_d ||
        cube1->aabb_z+cube1->aabb_d < cube2->aabb_z
    ){
        // no collision
        return (vec3_t){.x=0, .y=0, .z=0};
    }
    
    uint8_t i;
    uint8_t j;

    float t;
    float cube1_min;
    float cube1_max;
    float cube2_min;
    float cube2_max;

    float overlap;
    int8_t overlap_flip; // overlap flipping is used so the resolution will be for cube1 instead of cube2
    float min_overlap;
    int8_t min_overlap_flip;
    int8_t min_overlap_axis;

    // <axes array>
        vec3_t axes[15] = {
            // 3 axes of cube1
            (vec3_t){
                cube1->vertices[0].x - cube1->vertices[1].x,
                cube1->vertices[0].y - cube1->vertices[1].y,
                cube1->vertices[0].z - cube1->vertices[1].z,
            },
            (vec3_t){
                cube1->vertices[0].x - cube1->vertices[2].x,
                cube1->vertices[0].y - cube1->vertices[2].y,
                cube1->vertices[0].z - cube1->vertices[2].z,
            },
            (vec3_t){
                cube1->vertices[0].x - cube1->vertices[4].x,
                cube1->vertices[0].y - cube1->vertices[4].y,
                cube1->vertices[0].z - cube1->vertices[4].z,
            },
            // 3 axes of cube2
            (vec3_t){
                cube2->vertices[0].x - cube2->vertices[1].x,
                cube2->vertices[0].y - cube2->vertices[1].y,
                cube2->vertices[0].z - cube2->vertices[1].z,
            },
            (vec3_t){
                cube2->vertices[0].x - cube2->vertices[2].x,
                cube2->vertices[0].y - cube2->vertices[2].y,
                cube2->vertices[0].z - cube2->vertices[2].z,
            },
            (vec3_t){
                cube2->vertices[0].x - cube2->vertices[4].x,
                cube2->vertices[0].y - cube2->vertices[4].y,
                cube2->vertices[0].z - cube2->vertices[4].z,
            }
        };
        // 9 axes of cross products between cube1 axes and cube2 axes
        axes[6]  = vec3_cross_product(axes[0],axes[3]);
        axes[7]  = vec3_cross_product(axes[0],axes[4]);
        axes[8]  = vec3_cross_product(axes[0],axes[5]);
        axes[9]  = vec3_cross_product(axes[1],axes[3]);
        axes[10] = vec3_cross_product(axes[1],axes[4]);
        axes[11] = vec3_cross_product(axes[1],axes[5]);
        axes[12] = vec3_cross_product(axes[2],axes[3]);
        axes[13] = vec3_cross_product(axes[2],axes[4]);
        axes[14] = vec3_cross_product(axes[2],axes[5]);

        // normalize all axes
        for (i = 0; i < 15; i++) {
            float i_mag = 1/sqrt(axes[i].x*axes[i].x + axes[i].y*axes[i].y + axes[i].z*axes[i].z);
            axes[i].x *= i_mag;
            axes[i].y *= i_mag;
            axes[i].z *= i_mag;
        }
    // </axes array>

    for (i = 0; i < 15; i++) {
        // skip 0 vectors that may acore from cross products
        if (axes[i].x == 0 && axes[i].y == 0 && axes[i].z == 0) {
            continue;
        }

        // project cube1 verts
        for (j = 0; j < 8; j++) {
            // project vert
            // computes for (axes[i].x*t,axes[i].y*t,axes[i].z*t) is the closest point on the axis to the vert
            t = (axes[i].x*cube1->vertices[j].x + axes[i].y*cube1->vertices[j].y + axes[i].z*cube1->vertices[j].z);
            
            if (j == 0) {
                cube1_min = t;
                cube1_max = t;
            }else {
                cube1_min = fmin(cube1_min, t);
                cube1_max = fmax(cube1_max, t);
            }
        }
        // project cube2 verts
        for (j = 0; j < 8; j++) {
            // project vert
            // computes for (axes[i].x*t,axes[i].y*t,axes[i].z*t) is the closest point on the axis to the vert
            t = (axes[i].x*cube2->vertices[j].x + axes[i].y*cube2->vertices[j].y + axes[i].z*cube2->vertices[j].z);

            if (j == 0) {
                cube2_min = t;
                cube2_max = t;
            }else {
                cube2_min = fmin(cube2_min, t);
                cube2_max = fmax(cube2_max, t);
            }
        }
        // no overlap - no collision
        if (cube1_min > cube2_max || cube1_max < cube2_min){
            return (vec3_t){.x=0, .y=0, .z=0};
        }

        // calculate overlap
        if (cube1_max <= cube2_max) {
            if (cube1_min >= cube2_min) {
                // cube1 encapsulated in cube2
                overlap = cube1_max - cube1_min;
                // add exit overlap
                if (cube2_max-cube1_max < cube1_min-cube2_min) {
                    overlap += cube2_max-cube1_max;
                    overlap_flip = 0;
                }else {
                    overlap += cube1_min - cube2_min;
                    overlap_flip = 1;
                }
            }else {
                overlap = cube1_max - cube2_min;
                overlap_flip = 1;
            }
        }else if (cube1_min >= cube2_min) {
            overlap = cube2_max - cube1_min;
            overlap_flip = 0;
        }else {
            // cube2 encapsulated in cube1
            overlap = cube2_max - cube2_min;
            // add exit overlap
            if (cube1_max-cube2_max < cube2_min-cube1_min) {
                overlap += cube1_max-cube2_max;
                overlap_flip = 0;
            }else {
                overlap += cube2_min-cube1_min;
                overlap_flip = 1;
            }
        }

        if (i == 0 || overlap < min_overlap) {
            min_overlap = overlap;
            min_overlap_axis = i;
            min_overlap_flip = overlap_flip;
        }
    }
    
    if (min_overlap_flip == 1) {
        min_overlap *= -1;
    }

    return (vec3_t){
        .x = axes[min_overlap_axis].x*min_overlap,
        .y = axes[min_overlap_axis].y*min_overlap,
        .z = axes[min_overlap_axis].z*min_overlap
    };
}