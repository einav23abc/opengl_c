// simple passthrough shader

in vec3 in_vertex_position;
in vec2 in_vertex_texcoord;

//varying vec3 v_position;
varying vec2 v_texcoord;

uniform mat4 u_camera_world_view_projection_matrix;

void main(){
    vec4 position = vec4(in_vertex_position,1.0)*u_camera_world_view_projection_matrix;
    //v_position = position.xyz/position.w;
    v_texcoord = in_vertex_texcoord;
    gl_Position = position;
}