static char screen_quad_mesh_frag_arr[] =
"   #version 150\n"
"	varying vec2 v_vertex_data;	\n"
"		\n"
"	uniform sampler2D u_texture;	\n"
"		\n"
"	void main(){	\n"
"	    gl_FragColor = texture2D(u_texture, v_vertex_data);	\n"
"	}	\n"
;
static char* screen_quad_mesh_frag = screen_quad_mesh_frag_arr;

static char screen_quad_mesh_vert_arr[] =
"   #version 150\n"
"	in vec2 in_vertex_data;	\n"
"		\n"
"	varying vec2 v_vertex_data;	\n"
"		\n"
"	void main(){	\n"
"	    v_vertex_data = in_vertex_data;	\n"
"	    gl_Position = vec4(in_vertex_data*2.0-1.0, 1.0, 1.0);	\n"
"	}	\n"
;
static char* screen_quad_mesh_vert = screen_quad_mesh_vert_arr;