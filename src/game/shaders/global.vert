in vec3 in_vertex_position;
in vec2 in_vertex_texcoord;
in vec3 in_vertex_normal;

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

uniform mat4 u_camera_world_view_projection_matrix;

uniform vec3 u_position;
uniform vec3 u_scale;
uniform vec3 u_rotation;

void main(){
    mat3 rot_x_mat = mat3(
        1, 0,                  0,
        0, cos(u_rotation.x), -sin(u_rotation.x),
        0, sin(u_rotation.x),  cos(u_rotation.x)
    );
    mat3 rot_y_mat = mat3(
         cos(u_rotation.y), 0, sin(u_rotation.y),
         0,                 1, 0,
        -sin(u_rotation.y), 0, cos(u_rotation.y)
    );
    mat3 rot_z_mat = mat3(
        cos(u_rotation.z), -sin(u_rotation.z), 0,
        sin(u_rotation.z),  cos(u_rotation.z), 0,
        0,                  0,                 1
    );
    mat3 rot_mat = rot_y_mat*rot_z_mat*rot_x_mat;
    vec3 position = in_vertex_position*u_scale*rot_mat + u_position;
    vec3 normal = in_vertex_normal*rot_mat;

    v_texcoord = in_vertex_texcoord;
    v_position = position;
    v_normal = normal;
    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;
}