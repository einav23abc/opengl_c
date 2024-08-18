varying vec2 v_texcoord;

uniform float u_rads;

#define M_PI 3.1415926535897932384626433832795

void main(){
    vec4 color = vec4(0.0);

    vec2 position = v_texcoord*2.0-1.0;
    if (dot(position,position) <= 1.0) {
        color = vec4(1.0);

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

        if (ang <= u_rads) {
            color = vec4(0.5, 0.5, 0.5, 1.0);
        }
    }

    gl_FragColor = color;
}