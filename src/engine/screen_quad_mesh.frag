varying vec2 v_vertex_data;

uniform sampler2D u_texture;

void main(){
    gl_FragColor = texture2D(u_texture, v_vertex_data);
}