#version 150

varying vec2 v_texcoord;

uniform sampler2D u_texture;

/* x = x translation
 * y = y translation
 * z = width scale
 * w = height scale
 */
uniform vec4 u_texcoord_change;

void main(){
    vec2 texcoord = v_texcoord*u_texcoord_change.zw + u_texcoord_change.xy;

    gl_FragColor = texture2D(u_texture, texcoord);
}