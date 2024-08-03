#define MAX_JOINTS (20)

in vec3 in_vertex_position;
in vec2 in_vertex_texcoord;
in vec3 in_vertex_normal;
in ivec3 in_vertex_joint_id;
in vec3 in_vertex_joint_wheight;

uniform mat4 u_camera_world_view_projection_matrix;
uniform mat4 u_joint_matrices[MAX_JOINTS];

uniform vec3 u_position;
uniform vec3 u_scale;
uniform vec4 u_quat_rotation;

vec4 quat_multiply(vec4 q1, vec4 q2) {
    return vec4(
        (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y),
        (q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x),
        (q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w),
        (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z)
    );
}
vec4 quat_conjugate(vec4 q) {
    return vec4(-q.xyz, q.w);
}
vec3 rotate_vector(vec3 vec, vec4 q) {
    vec4 q_vec = vec4(vec, 0);
    vec4 q_res = quat_multiply(quat_multiply(q,q_vec),quat_conjugate(q));
    return q_res.xyz;
}

void main() {
    vec3 model_position;

    if (in_vertex_joint_wheight[0] == 0 && in_vertex_joint_wheight[1] == 0 && in_vertex_joint_wheight[2] == 0) {
        model_position = in_vertex_position;
    }else {
        mat4 m;
        for (uint i = 0; i < 3; i++) {
            m += in_vertex_joint_wheight[i] * u_joint_matrices[in_vertex_joint_id[i]];
        }
        model_position = vec3(vec4(in_vertex_position,1.0) * m);
    }

    vec3 position = rotate_vector(model_position*u_scale, u_quat_rotation) + u_position;
    
    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;
}