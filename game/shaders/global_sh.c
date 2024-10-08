static char global_frag_arr[] =
"	varying vec3 v_position;\n"
"	varying vec2 v_texcoord;\n"
"	varying vec3 v_normal;\n"
"	\n"
"	uniform sampler2D u_texture;\n"
"	uniform sampler2D u_sun_shadow_map_texture;\n"
"	\n"
"	uniform vec3 u_sun_vector;\n"
"	uniform mat4 u_sun_shadow_map_wvp_mat;\n"
"	\n"
"	uniform vec3 u_camera_position;\n"
"	\n"
"	const vec2 poisson_sampling_disk[16] = vec2[](\n"
"	    vec2(-0.94201624, -0.39906216 ),\n"
"	    vec2( 0.94558609, -0.76890725 ),\n"
"	    vec2(-0.094184101,-0.92938870 ),\n"
"	    vec2( 0.34495938,  0.29387760 ),\n"
"	    vec2(-0.91588581,  0.45771432 ),\n"
"	    vec2(-0.81544232, -0.87912464 ),\n"
"	    vec2(-0.38277543,  0.27676845 ),\n"
"	    vec2( 0.97484398,  0.75648379 ),\n"
"	    vec2( 0.44323325, -0.97511554 ),\n"
"	    vec2( 0.53742981, -0.47373420 ),\n"
"	    vec2(-0.26496911, -0.41893023 ),\n"
"	    vec2( 0.79197514,  0.19090188 ),\n"
"	    vec2(-0.24188840,  0.99706507 ),\n"
"	    vec2(-0.81409955,  0.91437590 ),\n"
"	    vec2( 0.19984126,  0.78641367 ),\n"
"	    vec2( 0.14383161, -0.14100790 )\n"
"	);\n"
"	\n"
"	float random(vec3 seed, int i) {\n"
"	    vec4 seed4 = vec4(seed, i);\n"
"	    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));\n"
"	    return fract(sin(dot_product) * 43758.5453);\n"
"	}\n"
"	\n"
"	void main(){\n"
"	    float normal_dot_sun = dot(v_normal,-u_sun_vector);\n"
"	    float light = max(0.0,normal_dot_sun);\n"
"	\n"
"	    vec3 sun_shadow_map_position = (vec4(v_position, 1.0)*u_sun_shadow_map_wvp_mat)*0.5+0.5;\n"
"	\n"
"	    float sun_current_depth = sun_shadow_map_position.z;\n"
"	    float bias = max(0.00005, 0.0002 * (1.0 - normal_dot_sun));\n"
"	    float shadow = 1.0;\n"
"	    for (int i = 0; i < 4; i++) {\n"
"	        int index = int(16.0*random(gl_FragCoord.xyy, i))%16;\n"
"	        float sun_closest_depth = texture2D(u_sun_shadow_map_texture, sun_shadow_map_position.xy + poisson_sampling_disk[index]/(1080.0*2.0)).r;\n"
"	        if (sun_closest_depth + bias < sun_current_depth) {\n"
"	            shadow -= 0.25;\n"
"	        }\n"
"	    }\n"
"	    \n"
"	    float lighting = clamp(light*shadow,0.0,1.0)*0.9+0.1;\n"
"	\n"
"	    float aerial_mixing = max(0.0,min(1.0,1-0.001*distance(v_position,u_camera_position)));\n"
"	    const vec3 aerial_color = vec3(0.2, 0.2, 0.3);\n"
"	\n"
"	    vec3 color = texture2D(u_texture, v_texcoord).xyz*lighting;\n"
"	    color = (color*aerial_mixing)+(aerial_color*(1-aerial_mixing));\n"
"	\n"
"	    gl_FragColor = vec4(color,1.0);\n"
"	    \n"
"	    // gl_FragColor = vec4(v_normal*0.5+0.5,1.0);\n"
"	}\n"
;
static char* global_frag = global_frag_arr;

static char global_vert_arr[] =
"	#define MAX_JOINTS (20)\n"
"	\n"
"	in vec3 in_vertex_position;\n"
"	in vec2 in_vertex_texcoord;\n"
"	in vec3 in_vertex_normal;\n"
"	in ivec3 in_vertex_joint_id;\n"
"	in vec3 in_vertex_joint_wheight;\n"
"	\n"
"	varying vec3 v_position;\n"
"	varying vec2 v_texcoord;\n"
"	varying vec3 v_normal;\n"
"	\n"
"	uniform mat4 u_camera_world_view_projection_matrix;\n"
"	uniform mat4 u_joint_matrices[MAX_JOINTS];\n"
"	\n"
"	uniform vec3 u_position;\n"
"	uniform vec3 u_scale;\n"
"	uniform vec4 u_quat_rotation;\n"
"	\n"
"	vec4 quat_multiply(vec4 q1, vec4 q2) {\n"
"	    return vec4(\n"
"	        (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y),\n"
"	        (q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x),\n"
"	        (q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w),\n"
"	        (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z)\n"
"	    );\n"
"	}\n"
"	vec4 quat_conjugate(vec4 q) {\n"
"	    return vec4(-q.xyz, q.w);\n"
"	}\n"
"	vec3 rotate_vector(vec3 vec, vec4 q) {\n"
"	    vec4 q_vec = vec4(vec, 0);\n"
"	    vec4 q_res = quat_multiply(quat_multiply(q,q_vec),quat_conjugate(q));\n"
"	    return q_res.xyz;\n"
"	}\n"
"	\n"
"	void main() {\n"
"	    vec3 model_position;\n"
"	    vec3 model_normal;\n"
"	\n"
"	    if (in_vertex_joint_wheight[0] == 0 && in_vertex_joint_wheight[1] == 0 && in_vertex_joint_wheight[2] == 0) {\n"
"	        model_position = in_vertex_position;\n"
"	        model_normal = in_vertex_normal;\n"
"	    }else {\n"
"	        mat4 m;\n"
"	        for (uint i = 0; i < 3; i++) {\n"
"	            m += in_vertex_joint_wheight[i] * u_joint_matrices[in_vertex_joint_id[i]];\n"
"	        }\n"
"	        model_position = vec3(vec4(in_vertex_position,1.0) * m);\n"
"	        model_normal = in_vertex_normal * mat3(m);\n"
"	    }\n"
"	\n"
"	    vec3 position = rotate_vector(model_position*u_scale, u_quat_rotation) + u_position;\n"
"	    vec3 normal = rotate_vector(model_normal, u_quat_rotation);\n"
"	\n"
"	    v_texcoord = in_vertex_texcoord;\n"
"	    v_position = position;\n"
"	    v_normal = normal;\n"
"	    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;\n"
"	}\n"
;
static char* global_vert = global_vert_arr;