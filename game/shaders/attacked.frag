#version 150

varying vec2 v_texcoord;

uniform sampler2D u_texture;

uniform int u_frames_amount;
uniform int u_current_frame;

void main(){
    int current_frame = min(u_current_frame, u_frames_amount-1);

    vec2 texcoord = vec2(
        (v_texcoord.x + float(current_frame))/float(u_frames_amount),
        v_texcoord.y
    );
    vec4 color = texture2D(u_texture, texcoord);

    if (color.a <= 0.1) {
        vec2 position = v_texcoord*2.0-1.0;
        float dist_from_center = sqrt(dot(position,position));
        float min_dist = min(1.0, float(current_frame)/float(u_frames_amount-1));
        float max_dist = min(1.0, 2.0*float(current_frame)/float(u_frames_amount-1));
        if (dist_from_center >= min_dist && dist_from_center <= max_dist) {
            color = vec4(28.0, 22.0, 24.0, 255)/255.0;
        }
    }

    gl_FragColor = color;
}