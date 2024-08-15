static char cube_frag_arr[] =
"	varying vec2 v_texcoord;	\n"
"		\n"
"	uniform vec4 u_color;	\n"
"		\n"
"	void main(){	\n"
"	    gl_FragColor = u_color;	\n"
"	}	\n"
;
static char* cube_frag = cube_frag_arr;

static char cube_vert_arr[] =
"	in vec3 in_vertex_position;	\n"
"	in vec2 in_vertex_texcoord;	\n"
"		\n"
"	varying vec2 v_texcoord;	\n"
"		\n"
"	uniform mat4 u_camera_world_view_projection_matrix;	\n"
"		\n"
"	uniform vec3 u_position;	\n"
"	uniform vec3 u_size;	\n"
"		\n"
"	void main(){	\n"
"	    vec4 position = vec4( in_vertex_position*u_size + u_position , 1.0 )*u_camera_world_view_projection_matrix;	\n"
"	    v_texcoord = in_vertex_texcoord;	\n"
"	    gl_Position = position;	\n"
"	}	\n"
;
static char* cube_vert = cube_vert_arr;