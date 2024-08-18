in vec3 in_vertex_position;
in vec2 in_vertex_texcoord;

varying vec2 v_texcoord;

uniform mat4 u_camera_world_view_projection_matrix;

uniform vec2 u_position;
uniform vec2 u_scale;

void main() {
    vec3 model_position = in_vertex_position;

    vec3 position = model_position*vec3(u_scale,1.0) + vec3(u_position,0.0);

    v_texcoord = in_vertex_texcoord;

    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;
}