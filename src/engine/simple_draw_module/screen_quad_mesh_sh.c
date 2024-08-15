char screen_quad_mesh_frag[] =
"	varying vec2 v_vertex_data;	\n"
"		\n"
"	uniform sampler2D u_texture;	\n"
"		\n"
"	void main(){	\n"
"	    gl_FragColor = texture2D(u_texture, v_vertex_data);	\n"
"	}	\n"
;

char screen_quad_mesh_vert[] =
"	in vec2 in_vertex_data;	\n"
"		\n"
"	varying vec2 v_vertex_data;	\n"
"		\n"
"	void main(){	\n"
"	    v_vertex_data = in_vertex_data;	\n"
"	    gl_Position = vec4(in_vertex_data*2.0-1.0, 1.0, 1.0);	\n"
"	}	\n"
;