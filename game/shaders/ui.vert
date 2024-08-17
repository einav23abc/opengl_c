in vec3 in_vertex_position;
in vec2 in_vertex_texcoord;

varying vec3 v_position;
varying vec2 v_texcoord;

uniform mat4 u_camera_world_view_projection_matrix;

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
    vec3 model_position = in_vertex_position;

    vec3 position = rotate_vector(model_position*u_scale, u_quat_rotation) + u_position;

    v_texcoord = in_vertex_texcoord;

    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;
}