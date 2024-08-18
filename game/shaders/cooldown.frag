varying vec2 v_texcoord;

uniform float u_rads;

#define M_PI 3.1415926535897932384626433832795

void main(){
    vec2 position = v_texcoord*2.0-1.0;

    float dist_from_center = sqrt(dot(position,position));
    float ang = atan(position.y/position.x);
    if (position.x <= 0.0) {
        ang += M_PI;
    }
    // rotate
    ang += M_PI*0.5;
    // keep positive
    if (ang < 0) {
        ang += M_PI*2.0;
    }

    
    vec4 color = vec4(1.0);
    const vec3 color_empty = vec3(138, 143, 196)/255.0;
    const vec3 color_full = vec3(161, 61, 59)/255.0;
    const vec3 color_edge = vec3(28, 22, 24)/255.0;
    const vec3 color_button = vec3(28, 22, 24)/255.0;
    const vec3 color_line = vec3(28, 22, 24)/255.0;
    const float line_size = 0.15;

    if (dist_from_center <= 0.15) {
        color.rgb = color_button;
    }else if (dist_from_center <= 0.85) {
        color.rgb = color_full;
        if (
            ang + M_PI*0.2*line_size/dist_from_center > u_rads &&
            ang - M_PI*0.2*line_size/dist_from_center < u_rads
        ) {
            color.rgb = color_line;
        }else if (ang <= u_rads) {
            color.rgb = color_empty;
        }
    }else if (dist_from_center <= 1.0) {
        color.rgb = color_edge;
    }else {
        color.a = 0.0;
    }

    gl_FragColor = color;
}