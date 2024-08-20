#version 150

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

// uniform sampler2D u_texture;

uniform vec3 u_color;
uniform float u_speed;
uniform float u_freq;
uniform float u_time;
uniform float u_width;
uniform float u_length;

#define M_PI 3.1415926535897932384626433832795

void main(){
    float y = 1.0-v_texcoord.y;
    
    if (u_width > 0.0 && u_width <= 1.0 && u_length > 0.0){
        float min_y = u_time/u_length;
        float max_y = u_time/u_length + u_width;
        if (y < min_y || y > max_y) {
            discard;
        }
    }

    float alpha = ceil(sin((y - u_time*u_speed)*M_PI*u_freq)*0.5+0.5 - y);
    if (alpha <= 0.0) {
        discard;
    }


    gl_FragColor = vec4(u_color,1.0);
}