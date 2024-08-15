in vec2 in_vertex_data;

varying vec2 v_vertex_data;

void main(){
    v_vertex_data = in_vertex_data;
    gl_Position = vec4(in_vertex_data*2.0-1.0, 1.0, 1.0);
}