#version 150

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

uniform sampler2D u_texture;
uniform sampler2D u_sun_shadow_map_texture;

uniform vec3 u_sun_vector;
uniform mat4 u_sun_shadow_map_wvp_mat;

uniform vec3 u_camera_position;

const vec2 poisson_sampling_disk[16] = vec2[](
    vec2(-0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2(-0.094184101,-0.92938870 ),
    vec2( 0.34495938,  0.29387760 ),
    vec2(-0.91588581,  0.45771432 ),
    vec2(-0.81544232, -0.87912464 ),
    vec2(-0.38277543,  0.27676845 ),
    vec2( 0.97484398,  0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2(-0.26496911, -0.41893023 ),
    vec2( 0.79197514,  0.19090188 ),
    vec2(-0.24188840,  0.99706507 ),
    vec2(-0.81409955,  0.91437590 ),
    vec2( 0.19984126,  0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
);

float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed, i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

void main(){
    float normal_dot_sun = dot(v_normal,-u_sun_vector);
    float light = max(0.0,normal_dot_sun);

    vec3 sun_shadow_map_position = ((vec4(v_position, 1.0)*u_sun_shadow_map_wvp_mat)*0.5+0.5).xyz;

    float sun_current_depth = sun_shadow_map_position.z;
    float bias = max(0.000025, 0.0001 * (1.0 - normal_dot_sun));
    float shadow = 1.0;
    for (int i = 0; i < 4; i++) {
        int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
        float sun_closest_depth = texture2D(u_sun_shadow_map_texture, sun_shadow_map_position.xy + poisson_sampling_disk[index]/(1080.0*2.0)).r;
        if (sun_closest_depth + bias < sun_current_depth) {
            shadow -= 0.25;
        }
    }
    
    float lighting = (clamp(light*shadow,0.0,1.0)*0.8+0.2);

    float aerial_mixing = max(0.0,min(1.0, 1.0 - 0.001*distance(v_position,u_camera_position)));
    const vec3 aerial_color = vec3(71.0, 65.0, 107.0)/255.0;

    vec4 color = texture2D(u_texture, v_texcoord);
    if (color.a <= 0.01) {
        discard;
    }
    color.xyz = color.xyz*lighting;
    color.xyz = (color.xyz*aerial_mixing)+(aerial_color*(1-aerial_mixing));

    gl_FragColor = color;
    
    // gl_FragColor = vec4(v_normal*0.5+0.5,1.0);
}