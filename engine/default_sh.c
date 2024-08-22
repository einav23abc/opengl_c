static char default_frag_arr[] =
"   #version 150\n"
"	// simple passthrough shader	\n"
"		\n"
"	//varying vec3 v_position;	\n"
"	varying vec2 v_texcoord;	\n"
"		\n"
"	uniform sampler2D u_texture;	\n"
"		\n"
"	void main(){	\n"
"	    gl_FragColor = texture2D( u_texture, v_texcoord );	\n"
"	}	\n"
;
static char* default_frag = default_frag_arr;

static char default_vert_arr[] =
"   #version 150\n"
"	// simple passthrough shader	\n"
"		\n"
"	in vec3 in_vertex_position;	\n"
"	in vec2 in_vertex_texcoord;	\n"
"		\n"
"	//varying vec3 v_position;	\n"
"	varying vec2 v_texcoord;	\n"
"		\n"
"	uniform mat4 u_camera_world_view_projection_matrix;	\n"
"		\n"
"	void main(){	\n"
"	    vec4 position = vec4(in_vertex_position,1.0)*u_camera_world_view_projection_matrix;	\n"
"	    //v_position = position.xyz/position.w;	\n"
"	    v_texcoord = in_vertex_texcoord;	\n"
"	    gl_Position = position;	\n"
"	}	\n"
;
static char* default_vert = default_vert_arr;