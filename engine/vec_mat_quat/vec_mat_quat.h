#ifndef VEC_MAT_QUAT_H
#define VEC_MAT_QUAT_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>


typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    int32_t x;
    int32_t y;
} ivec2_t;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} ivec3_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} uvec2_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
} uvec3_t;

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quat_t;

typedef struct {
    quat_t rot;
    vec3_t pos;
    vec3_t scale;
} quat_vec_vec_t;

typedef struct {
    float mat[9];
} mat3_t;

typedef struct {
    float mat[16];
} mat4_t;



vec3_t vec3_mul(vec3_t vec1, vec3_t vec2);
vec3_t vec3_add(vec3_t vec1, vec3_t vec2);

float dot_product(vec3_t vec1, vec3_t vec2);
vec3_t cross_product(vec3_t vec1, vec3_t vec2);

mat4_t mat4_from_mat3(mat3_t mat);
// \returns mat4 with last row and coloumn removed
mat3_t mat3_from_mat4(mat4_t mat);

//  assumes:
//      mat1 width  = mat2 height = m
//      mat3 width  = mat2 width  = n
//      mat3 height = mat1 height = i
void mat_mul(float* mat1, float* mat2, float* restrict mat3, uint8_t m, uint8_t i, uint8_t n);

mat3_t mat3_mul(mat3_t mat1, mat3_t mat2);
mat4_t mat4_mul(mat4_t mat1, mat4_t mat2);
vec3_t vec3_mul_by_mat3(vec3_t vec, mat3_t mat);

mat3_t mat3_mul_by_scalar(mat3_t mat, float scalar);
mat4_t mat4_mul_by_scalar(mat4_t mat, float scalar);

//  assumes:
//      mat1 width  = mat2 width  = mat3 width  = w
//      mat1 height = mat3 height = mat3 height = h 
void mat_add(float* mat1, float* mat2, float* mat3, uint8_t w, uint8_t h);
mat3_t mat3_add(mat3_t mat1, mat3_t mat2);
mat4_t mat4_add(mat4_t mat1, mat4_t mat2);


quat_t quat_multiply(quat_t q1, quat_t q2);
quat_t quat_conjugate(quat_t q);

quat_t quat_slerp(quat_t q1, quat_t q2, float t);

mat3_t mat3_from_axis_angle(vec3_t axis, float ang);

quat_t quat_from_axis_angle(vec3_t vec, float ang);
quat_t quat_from_axis_angles_yzx(float rx, float ry, float rz);
quat_t quat_from_euler_angles(float roll, float pitch, float yaw);

vec3_t rotate_vector(vec3_t vec, quat_t q);
vec3_t vec_scale_rotate_translate(vec3_t vec, quat_vec_vec_t qvv);

mat3_t rot_mat3_from_quat(quat_t q);
quat_t quat_from_rot_mat3(mat3_t mat);
quat_vec_vec_t quat_vec_vec_from_mat4(mat4_t mat);
mat4_t mat4_from_quat_vec_vec(quat_vec_vec_t qvv);

void print_mat3(mat3_t mat);
void print_mat4(mat4_t mat);
void print_quat_vec_vec(quat_vec_vec_t qvv);


#endif