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

    gl_FragColor = color;
}