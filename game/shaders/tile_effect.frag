varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

// uniform sampler2D u_texture;

uniform vec3 u_color;
uniform float u_speed;
uniform float u_freq;
uniform float u_time;

#define M_PI 3.1415926535897932384626433832795

void main(){
    float y = 1.0-v_texcoord.y;
    float alpha = max(0.0, ceil(sin((y - u_time*u_speed)*M_PI*u_freq)*0.5+0.5 - y) );

    vec4 color = vec4(u_color,1.0)*alpha;

    gl_FragColor = color;
}