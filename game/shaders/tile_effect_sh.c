char tile_effect_vert_sh[] = "#version 150\n#define MAX_JOINTS (20)\n\nin vec3 in_vertex_position;\nin vec2 in_vertex_texcoord;\n\nvarying vec3 v_position;\nvarying vec2 v_texcoord;\n\nuniform mat4 u_camera_world_view_projection_matrix;\n\nuniform vec3 u_position;\nuniform vec3 u_scale;\nuniform vec4 u_quat_rotation;\n\nvec4 quat_multiply(vec4 q1, vec4 q2) {\n    return vec4(\n        (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y),\n        (q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x),\n        (q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w),\n        (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z)\n    );\n}\nvec4 quat_conjugate(vec4 q) {\n    return vec4(-q.xyz, q.w);\n}\nvec3 rotate_vector(vec3 vec, vec4 q) {\n    vec4 q_vec = vec4(vec, 0);\n    vec4 q_res = quat_multiply(quat_multiply(q,q_vec),quat_conjugate(q));\n    return q_res.xyz;\n}\n\nvoid main() {\n    vec3 model_position = in_vertex_position;\n\n    vec3 position = rotate_vector(model_position*u_scale, u_quat_rotation) + u_position;\n\n    v_texcoord = in_vertex_texcoord;\n    v_position = position;\n    gl_Position = vec4(position, 1.0)*u_camera_world_view_projection_matrix;\n}";
char tile_effect_frag_sh[] = "#version 150\n\nvarying vec3 v_position;\nvarying vec2 v_texcoord;\nvarying vec3 v_normal;\n\n// uniform sampler2D u_texture;\n\nuniform vec3 u_color;\nuniform float u_speed;\nuniform float u_freq;\nuniform float u_time;\nuniform float u_width;\nuniform float u_length;\n\n#define M_PI 3.1415926535897932384626433832795\n\nvoid main(){\n    float y = 1.0-v_texcoord.y;\n    \n    if (u_width > 0.0 && u_width <= 1.0 && u_length > 0.0){\n        float min_y = u_time/u_length;\n        float max_y = u_time/u_length + u_width;\n        if (y < min_y || y > max_y) {\n            discard;\n        }\n    }\n\n    float alpha = ceil(sin((y - u_time*u_speed)*M_PI*u_freq)*0.5+0.5 - y);\n    if (alpha <= 0.0) {\n        discard;\n    }\n\n\n    gl_FragColor = vec4(u_color,1.0);\n}";