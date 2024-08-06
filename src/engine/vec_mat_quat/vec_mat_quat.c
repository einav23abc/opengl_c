#include "vec_mat_quat.h"


vec3_t vec3_mul(vec3_t vec1, vec3_t vec2) {
    return (vec3_t){
        .x = vec1.x*vec2.x,
        .y = vec1.y*vec2.y,
        .z = vec1.z*vec2.z
    };
}
vec3_t vec3_add(vec3_t vec1, vec3_t vec2) {
    return (vec3_t){
        .x = vec1.x+vec2.x,
        .y = vec1.y+vec2.y,
        .z = vec1.z+vec2.z
    };
}

float dot_product(vec3_t vec1, vec3_t vec2) {
    return (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
}
vec3_t cross_product(vec3_t vec1, vec3_t vec2) {
    return (vec3_t){
        .x = vec1.y*vec2.z - vec1.z*vec2.y,
        .y = vec1.z*vec2.x - vec1.x*vec2.z,
        .z = vec1.x*vec2.y - vec1.y*vec2.x
    };
}

mat4_t mat4_from_mat3(mat3_t mat) {
    return (mat4_t){
        .mat = {
            mat.mat[0], mat.mat[1], mat.mat[2], 0,
            mat.mat[3], mat.mat[4], mat.mat[5], 0,
            mat.mat[6], mat.mat[7], mat.mat[8], 0,
            0,          0,          0,          1
        }
    };
}
mat3_t mat3_from_mat4(mat4_t mat) {
    return (mat3_t){
        mat.mat[0], mat.mat[1], mat.mat[2],
        mat.mat[4], mat.mat[5], mat.mat[6],
        mat.mat[8], mat.mat[9], mat.mat[10]
    };
}

//  assumes:
//      mat1 width  = mat2 height = m
//      mat3 width  = mat2 width  = n
//      mat3 height = mat1 height = i
void mat_mul(float* mat1, float* mat2, float* restrict mat3, uint8_t m, uint8_t i, uint8_t n) {
    // https://en.wikipedia.org/wiki/Matrix_multiplication
    for (uint8_t j = 0; j < i; j++) {
        for (uint8_t k = 0; k < n; k++) {
            float sum = 0;
            for (uint8_t l = 0; l < m; l++) {
                sum += mat1[j*m+l]*mat2[l*n+k];
            }
            mat3[j*n+k] = sum;
        }
    }
    return;
}

mat3_t mat3_mul(mat3_t mat1, mat3_t mat2) {
    mat3_t res_mat;
    mat_mul(mat1.mat, mat2.mat, res_mat.mat, 3, 3, 3);
    return res_mat;
}
mat4_t mat4_mul(mat4_t mat1, mat4_t mat2) {
    mat4_t res_mat;
    mat_mul(mat1.mat, mat2.mat, res_mat.mat, 4, 4, 4);
    return res_mat;
}
vec3_t vec3_mul_by_mat3(vec3_t vec, mat3_t mat) {
    float vec_mat[3] = {
        vec.x,
        vec.y,
        vec.z
    };

    float res_mat[3];
    mat_mul(mat.mat, vec_mat, res_mat, 3, 3, 1);
    
    return (vec3_t){
        .x = res_mat[0],
        .y = res_mat[1],
        .z = res_mat[2]
    };
}

mat3_t mat3_mul_by_scalar(mat3_t mat, float scalar) {
    mat3_t res;
    for (uint8_t i = 0; i < 9; i++) {
        res.mat[i] = mat.mat[i]*scalar;
    }
    return res;
}
mat4_t mat4_mul_by_scalar(mat4_t mat, float scalar) {
    mat4_t res;
    for (uint8_t i = 0; i < 16; i++) {
        res.mat[i] = mat.mat[i]*scalar;
    }
    return res;
}

// * assumes:
// *    mat1 width  = mat2 width  = mat3 width  = w
// *    mat1 height = mat3 height = mat3 height = h 
void mat_add(float* mat1, float* mat2, float* mat3, uint8_t w, uint8_t h) {
    for (uint32_t i = 0; i < w*h; i++) {
        mat3[i] = mat1[i]+mat2[i];
    }
    return;
}
mat3_t mat3_add(mat3_t mat1, mat3_t mat2) {
    mat3_t res_mat;
    mat_add(mat1.mat, mat2.mat, res_mat.mat, 3, 3);
    return res_mat;
}
mat4_t mat4_add(mat4_t mat1, mat4_t mat2) {
    mat4_t res_mat;
    mat_add(mat1.mat, mat2.mat, res_mat.mat, 4, 4);
    return res_mat;
}

quat_t quat_multiply(quat_t q1, quat_t q2) {
    return (quat_t){
        .w = (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z),
        .x = (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y),
        .y = (q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x),
        .z = (q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w)
    };
}
quat_t quat_conjugate(quat_t q) {
    return (quat_t){
        .w = q.w,
        .x = -q.x,
        .y = -q.y,
        .z = -q.z
    };
}

quat_t quat_slerp(quat_t q1, quat_t q2, float t) {
    // https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    
    // Calculate angle between them
    float cos_half_theta = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
    if (cos_half_theta < 0) {
        q2.w = -q2.w;
        q2.x = -q2.x;
        q2.y = -q2.y;
        q2.z = -q2.z;
        cos_half_theta = -cos_half_theta;
    }
    
    // if qa=qb or qa=-qb then theta = 0 and we can return qa
    if (cos_half_theta >= 1) {
        return q1;
    }

    // Calculate temporary values
    float half_theta = acos(cos_half_theta);
    float sin_half_theta = sqrt(1 - cos_half_theta*cos_half_theta);
    
    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to qa or qb
    if (fabs(sin_half_theta) < 0.001){ // fabs is floating point absolute
        return (quat_t) {
            .w = (q1.w*0.5 + q2.w*0.5),
            .x = (q1.x*0.5 + q2.x*0.5),
            .y = (q1.y*0.5 + q2.y*0.5),
            .z = (q1.z*0.5 + q2.z*0.5)
        };
    }

    float ratio_a = sin((1 - t) * half_theta) / sin_half_theta;
    float ratio_b = sin(t * half_theta) / sin_half_theta; 
    //calculate Quaternion.
    return (quat_t) {
        .w = (q1.w*ratio_a + q2.w*ratio_b),
        .x = (q1.x*ratio_a + q2.x*ratio_b),
        .y = (q1.y*ratio_a + q2.y*ratio_b),
        .z = (q1.z*ratio_a + q2.z*ratio_b)
    };
}

mat3_t mat3_from_axis_angle(vec3_t axis, float ang) {
    float cosa = cos(ang);
    float sina = sin(ang);
    float omca = (1-cosa); // 1-cos(a)

    float xy = axis.x*axis.y;
    float xz = axis.x*axis.z;
    float yz = axis.y*axis.z;
    
    return (mat3_t){
        .mat = {
            cosa+axis.x*axis.x*omca, xy*omca-axis.z*sina,     xz*omca+axis.y*sina,
            xy*omca+axis.z*sina,     cosa+axis.y*axis.y*omca, yz*omca-axis.x*sina,
            xz*omca-axis.y*sina,     yz*omca+axis.x*sina,     cosa+axis.z*axis.z*omca
        }
    };
}

quat_t quat_from_axis_angle(vec3_t vec, float ang) {
    float sina = sin(ang*0.5);
    float i_mag = 1/sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    return (quat_t){
        .w = cos(ang*0.5),
        .x = vec.x*i_mag*sina,
        .y = vec.y*i_mag*sina,
        .z = vec.z*i_mag*sina,
    };
}
quat_t quat_from_axis_angles_yzx(float rx, float ry, float rz) {
    quat_t qx = (quat_t){
        .w = cos(rx*0.5),
        .x = sin(rx*0.5),
        .y = 0,
        .z = 0
    };
    quat_t qy = (quat_t){
        .w = cos(ry*0.5),
        .x = 0,
        .y = sin(ry*0.5),
        .z = 0
    };
    quat_t qz = (quat_t){
        .w = cos(rz*0.5),
        .x = 0,
        .y = 0,
        .z = sin(rz*0.5)
    };
    return quat_multiply(qy,quat_multiply(qz,qx));
}
quat_t quat_from_euler_angles(float roll, float pitch, float yaw) {
    float sinroll = sin(roll*0.5);
    float cosroll = cos(roll*0.5);
    float sinpitch = sin(pitch*0.5);
    float cospitch = cos(pitch*0.5);
    float sinyaw = sin(yaw*0.5);
    float cosyaw = cos(yaw*0.5);
    return (quat_t){
        .w = cosroll*cospitch*cosyaw + sinroll*sinpitch*sinyaw,
        .x = sinroll*cospitch*cosyaw - cosroll*sinpitch*sinyaw,
        .z = cosroll*sinpitch*cosyaw + sinroll*cospitch*sinyaw,
        .y = cosroll*cospitch*sinyaw - sinroll*sinpitch*cosyaw
    };
}

vec3_t rotate_vector(vec3_t vec, quat_t q) {
    quat_t q_vec = (quat_t){
        .w = 0,
        .x = vec.x,
        .y = vec.y,
        .z = vec.z
    };
    quat_t q_res = quat_multiply(quat_multiply(q,q_vec),quat_conjugate(q));
    return (vec3_t){
        .x = q_res.x,
        .y = q_res.y,
        .z = q_res.z
    };
}
vec3_t vec_scale_rotate_translate(vec3_t vec, quat_vec_vec_t qvv) {
    return vec3_add(rotate_vector(vec3_mul(vec, qvv.scale),qvv.rot),qvv.pos);
}

mat3_t rot_mat3_from_quat(quat_t q) {
    float wx = 2*q.w*q.x;
    float wy = 2*q.w*q.y;
    float wz = 2*q.w*q.z;
    float xx = 2*q.x*q.x;
    float xy = 2*q.x*q.y;
    float xz = 2*q.x*q.z;
    float yy = 2*q.y*q.y;
    float yz = 2*q.y*q.z;
    float zz = 2*q.z*q.z;

    return (mat3_t){
        .mat = {
            1-(yy+zz), xy-wz,     xz+wy,
            xy+wz,     1-(xx+zz), yz-wx,
            xz-wy,     yz+wx,     1-(xx+yy)
        }
    };
}
quat_t quat_from_rot_mat3(mat3_t mat) {
    float w = sqrt(1 + mat.mat[0] + mat.mat[4] + mat.mat[8])*0.5;
    float iw4 = 1/(4*w);

    return (quat_t){
        .w = w,
        .x = (mat.mat[7] - mat.mat[5])*iw4,
        .y = (mat.mat[2] - mat.mat[6])*iw4,
        .z = (mat.mat[3] - mat.mat[1])*iw4,
    };
}
quat_vec_vec_t quat_vec_vec_from_mat4(mat4_t mat) {
    return (quat_vec_vec_t){
        .pos = (vec3_t){
            .x = mat.mat[3],
            .y = mat.mat[7],
            .z = mat.mat[11]
        },
        .scale = (vec3_t){
            .x = sqrt(mat.mat[0]*mat.mat[0] + mat.mat[4]*mat.mat[4] + mat.mat[8]*mat.mat[8]),
            .y = sqrt(mat.mat[1]*mat.mat[1] + mat.mat[5]*mat.mat[5] + mat.mat[9]*mat.mat[9]),
            .z = sqrt(mat.mat[2]*mat.mat[2] + mat.mat[6]*mat.mat[6] + mat.mat[10]*mat.mat[10])
        },
        .rot = quat_from_rot_mat3(mat3_from_mat4(mat))
    };
}
mat4_t mat4_from_quat_vec_vec(quat_vec_vec_t qvv) {
    mat4_t rot = mat4_from_mat3(rot_mat3_from_quat(qvv.rot));
    mat4_t pos = (mat4_t){
        .mat = {
            1, 0, 0, qvv.pos.x,
            0, 1, 0, qvv.pos.y,
            0, 0, 1, qvv.pos.z,
            0, 0, 0, 1
        }
    };
    mat4_t scale = (mat4_t){
        .mat = {
            qvv.scale.x, 0, 0, 0,
            0, qvv.scale.y, 0, 0,
            0, 0, qvv.scale.z, 0,
            0, 0, 0, 1
        }
    };

    return mat4_mul(mat4_mul(pos, rot), scale);
}

void print_mat3(mat3_t mat) {
    printf(
        "[\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n]\n",
        mat.mat[0], mat.mat[1], mat.mat[2],
        mat.mat[3], mat.mat[4], mat.mat[5],
        mat.mat[6], mat.mat[7], mat.mat[8]
    );
}
void print_mat4(mat4_t mat) {
    printf(
        "[\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n]\n",
        mat.mat[0],  mat.mat[1],  mat.mat[2],  mat.mat[3],
        mat.mat[4],  mat.mat[5],  mat.mat[6],  mat.mat[7],
        mat.mat[8],  mat.mat[9],  mat.mat[10], mat.mat[11],
        mat.mat[12], mat.mat[13], mat.mat[14], mat.mat[15]
    );
}
void print_quat_vec_vec(quat_vec_vec_t qvv) {
    printf(
        "{\n\trot :\t%f\t%f\t%f\t%f\n\tscale :\t%f\t%f\t%f\n\tpos :\t%f\t%f\t%f\n}\n",
        qvv.rot.w,
        qvv.rot.x,
        qvv.rot.y,
        qvv.rot.z,
        qvv.scale.x,
        qvv.scale.y,
        qvv.scale.z,
        qvv.pos.x,
        qvv.pos.y,
        qvv.pos.z
    );
}