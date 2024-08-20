#version 150

varying vec3 v_position;
varying vec2 v_texcoord;

uniform vec3 u_color;
uniform float u_speed;
uniform float u_freq;
uniform float u_time;

#define M_PI 3.1415926535897932384626433832795

void main(){
    float alpha = mod((v_position.x + v_position.y + v_position.z)/u_freq + u_time*u_speed, 1.0) - 0.5;
    if (alpha <= 0.0) {
        discard;
    }
    gl_FragColor = vec4(u_color, 1.0);
}