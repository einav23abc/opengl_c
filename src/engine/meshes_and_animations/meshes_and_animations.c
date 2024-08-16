#include "meshes_and_animations.h"


#ifndef _MESHES_MAX_AMOUNT_
#define _MESHES_MAX_AMOUNT_ (128)
#endif
uint64_t meshes_amount = 0;
mesh_t* meshes_list[_MESHES_MAX_AMOUNT_];
const uint64_t MESHES_MAX_AMOUNT = _MESHES_MAX_AMOUNT_;

#ifndef _ANIMATIONS_MAX_AMOUNT_
#define _ANIMATIONS_MAX_AMOUNT_ (128)
#endif
uint64_t animations_amount = 0;
animation_t* animations_list[_ANIMATIONS_MAX_AMOUNT_];
const uint64_t ANIMATIONS_MAX_AMOUNT = _ANIMATIONS_MAX_AMOUNT_;



mesh_t* generate_mesh(vbo_data_t* vbo_datas_arr, uint32_t vbo_datas_arr_size, uint32_t* indices_array, uint32_t indices_count, uint8_t unbinded) {
    if (meshes_amount >= MESHES_MAX_AMOUNT) return NULL;

    mesh_t* mesh = malloc(sizeof(mesh_t));
    if (mesh == NULL) {
        printf("malloc failed at mesh generation.\n");
        return NULL;
    }

    mesh->vbos_amount = vbo_datas_arr_size;
    mesh->vbos = malloc(sizeof(uint32_t)*mesh->vbos_amount);
    if (mesh->vbos == NULL) {
        printf("malloc failed at mesh generation.\n");
        return mesh;
    }
    
    mesh->saved = 0;
    mesh->unbinded = unbinded;
    
    if (unbinded == 0) {
        // create the vertex array object
        glGenVertexArrays(1, &mesh->vao);
        glBindVertexArray(mesh->vao);

        for (uint32_t i = 0; i < mesh->vbos_amount; i++) {
            glGenBuffers(1, &(mesh->vbos[i]));
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, vbo_datas_arr[i].data_arr_size, vbo_datas_arr[i].data_arr, GL_STATIC_DRAW);
            if (vbo_datas_arr[i].type == GL_INT) {
                glVertexAttribIPointer(i, vbo_datas_arr[i].size, vbo_datas_arr[i].type, vbo_datas_arr[i].stride, (void*)0);
            }else {
                glVertexAttribPointer(i, vbo_datas_arr[i].size, vbo_datas_arr[i].type, GL_FALSE, vbo_datas_arr[i].stride, (void*)0);
            }
            glEnableVertexAttribArray(i);
            glVertexAttribDivisor(i, vbo_datas_arr[i].divisor);
        }
    }else {
        mesh->vbo_datas_arr = malloc(sizeof(vbo_data_t)*mesh->vbos_amount);
        if (mesh->vbo_datas_arr == NULL) {
            printf("malloc failed at mesh generation.\n");
            return mesh;
        }
        memcpy(mesh->vbo_datas_arr, vbo_datas_arr, sizeof(vbo_data_t)*mesh->vbos_amount);
        for (uint32_t i = 0; i < mesh->vbos_amount; i++) {
            mesh->vbo_datas_arr[i].data_arr = malloc(mesh->vbo_datas_arr[i].data_arr_size);
            if (mesh->vbo_datas_arr[i].data_arr == NULL) {
                printf("malloc failed at mesh generation.\n");
                return mesh;
            }
            memcpy(mesh->vbo_datas_arr[i].data_arr, vbo_datas_arr[i].data_arr, mesh->vbo_datas_arr[i].data_arr_size);
        }
    }

    mesh->indices_count = indices_count;
    mesh->indices_array = malloc(sizeof(uint32_t)*mesh->indices_count);
    if (mesh->indices_array == NULL) {
        printf("malloc failed at mesh generation.\n");
        return mesh;
    }
    memcpy(mesh->indices_array, indices_array, sizeof(uint32_t)*mesh->indices_count);

    // default joint values
    mesh->joints_amount = 0;
    mesh->joints = NULL;
    mesh->pose_joint_transform_matrices = NULL;

    *((uint64_t*)&mesh->mesh_index) = meshes_amount;
    meshes_list[meshes_amount] = mesh;
    meshes_amount += 1;

    return mesh;
}
void bind_mesh(mesh_t* mesh) {
    if (mesh->unbinded == 0) return;

    // create the vertex array object
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    for (uint32_t i = 0; i < mesh->vbos_amount; i++) {
        glGenBuffers(1, &(mesh->vbos[i]));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, mesh->vbo_datas_arr[i].data_arr_size, mesh->vbo_datas_arr[i].data_arr, GL_STATIC_DRAW);
        if (mesh->vbo_datas_arr[i].type == GL_INT) {
            glVertexAttribIPointer(i, mesh->vbo_datas_arr[i].size, mesh->vbo_datas_arr[i].type, mesh->vbo_datas_arr[i].stride, (void*)0);
        }else {
            glVertexAttribPointer(i, mesh->vbo_datas_arr[i].size, mesh->vbo_datas_arr[i].type, GL_FALSE, mesh->vbo_datas_arr[i].stride, (void*)0);
        }
        glEnableVertexAttribArray(i);
        glVertexAttribDivisor(i, mesh->vbo_datas_arr[i].divisor);
    }

    if (mesh->saved == 0) {
        for (uint32_t i = 0; i < mesh->vbos_amount; i++) {
            free(mesh->vbo_datas_arr[i].data_arr);
        }
        free(mesh->vbo_datas_arr);
    }

    mesh->unbinded = 0;
}

mesh_t* generate_2d_quad_mesh(  float min_x, float max_x, float min_y, float max_y,
                                float min_u, float max_u, float min_v, float max_v) {
    float vertices_position_arr[] = {
        min_x, min_y, 0,
        min_x, max_y, 0,
        max_x, max_y, 0,
        max_x, min_y, 0
    };
    float vertices_texcoord_arr[] = {
        min_u,max_v,
        min_u,min_v,
        max_u,min_v,
        max_u,max_v
    };
    vbo_data_t vbo_datas_arr[2] = {
        {
            .data_arr_size = sizeof(vertices_position_arr),
            .data_arr = (void*)vertices_position_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(vertices_texcoord_arr),
            .data_arr = (void*)vertices_texcoord_arr,
            .size = 2,
            .type = GL_FLOAT,
            .stride = 2*sizeof(float),
            .divisor = 0
        }
    };
    
    uint32_t indices_array[] = {
        0, 1, 2,
        0, 2, 3
    };
    return generate_mesh(vbo_datas_arr, 2, indices_array, 6, 0);
}
mesh_t* mesh_generate_ball(uint32_t divisions) {
    const float start_vertices_position_arr[] = {
        // Side A
            sqrt(8.0/9.0),  0,             -1.0/3.0,
        -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
        -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
        
        // Side B
            sqrt(8.0/9.0),  0,             -1.0/3.0,
        -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
            0,              0,              1,
        
        // Side C
            sqrt(8.0/9.0),  0,             -1.0/3.0,
        -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
            0,              0,              1,
        
        // Side D
        -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
        -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
            0,              0,              1
    };
    uint32_t vertices_amount = 12;

    const uint32_t start_indices_array[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9,10,11
    };
    uint32_t polygons_amount = 4;

    float* vertices_position_arr = malloc(sizeof(start_vertices_position_arr));
    memcpy(vertices_position_arr, start_vertices_position_arr, sizeof(start_vertices_position_arr));
    uint32_t* indices_array = malloc(sizeof(start_indices_array));
    memcpy(indices_array, start_indices_array, sizeof(start_indices_array));
    
    for (uint32_t divs = 0; divs < divisions; divs++) {
        float* new_vertices_position_arr = malloc(sizeof(float)*polygons_amount*4*3*3);
        if (new_vertices_position_arr == NULL) {
            break;
        }
        uint32_t* new_indices_array = malloc(sizeof(uint32_t)*polygons_amount*4*3);
        if (new_indices_array == NULL) {
            free(new_vertices_position_arr);
            break;
        }
        
        for (uint32_t p = 0; p < polygons_amount; p++) {
            // A
            new_vertices_position_arr[((p*4 )*3 )*3  ] = vertices_position_arr[indices_array[p*3  ]*3  ];
            new_vertices_position_arr[((p*4 )*3 )*3+1] = vertices_position_arr[indices_array[p*3  ]*3+1];
            new_vertices_position_arr[((p*4 )*3 )*3+2] = vertices_position_arr[indices_array[p*3  ]*3+2];

            new_vertices_position_arr[((p*4 )*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
            new_vertices_position_arr[((p*4 )*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
            new_vertices_position_arr[((p*4 )*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);
            
            new_vertices_position_arr[((p*4 )*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
            new_vertices_position_arr[((p*4 )*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
            new_vertices_position_arr[((p*4 )*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
            
            // B
            new_vertices_position_arr[((p*4+1)*3 )*3  ] = vertices_position_arr[indices_array[p*3+1]*3  ];
            new_vertices_position_arr[((p*4+1)*3 )*3+1] = vertices_position_arr[indices_array[p*3+1]*3+1];
            new_vertices_position_arr[((p*4+1)*3 )*3+2] = vertices_position_arr[indices_array[p*3+1]*3+2];

            new_vertices_position_arr[((p*4+1)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3  ]*3  ]);
            new_vertices_position_arr[((p*4+1)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3  ]*3+1]);
            new_vertices_position_arr[((p*4+1)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3  ]*3+2]);
            
            new_vertices_position_arr[((p*4+1)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
            new_vertices_position_arr[((p*4+1)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
            new_vertices_position_arr[((p*4+1)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
            
            // C
            new_vertices_position_arr[((p*4+2)*3 )*3  ] = vertices_position_arr[indices_array[p*3+2]*3  ];
            new_vertices_position_arr[((p*4+2)*3 )*3+1] = vertices_position_arr[indices_array[p*3+2]*3+1];
            new_vertices_position_arr[((p*4+2)*3 )*3+2] = vertices_position_arr[indices_array[p*3+2]*3+2];

            new_vertices_position_arr[((p*4+2)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3  ] + vertices_position_arr[indices_array[p*3  ]*3  ]);
            new_vertices_position_arr[((p*4+2)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+1] + vertices_position_arr[indices_array[p*3  ]*3+1]);
            new_vertices_position_arr[((p*4+2)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+2] + vertices_position_arr[indices_array[p*3  ]*3+2]);
            
            new_vertices_position_arr[((p*4+2)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
            new_vertices_position_arr[((p*4+2)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
            new_vertices_position_arr[((p*4+2)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);

            // MIDDLE
            new_vertices_position_arr[((p*4+3)*3 )*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
            new_vertices_position_arr[((p*4+3)*3 )*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
            new_vertices_position_arr[((p*4+3)*3 )*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);
            
            new_vertices_position_arr[((p*4+3)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
            new_vertices_position_arr[((p*4+3)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
            new_vertices_position_arr[((p*4+3)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
            
            new_vertices_position_arr[((p*4+3)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
            new_vertices_position_arr[((p*4+3)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
            new_vertices_position_arr[((p*4+3)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);


            // A
            new_indices_array[(p*4 )*3  ] = ((p*4 )*3 );
            new_indices_array[(p*4 )*3+1] = ((p*4 )*3+1);
            new_indices_array[(p*4 )*3+2] = ((p*4 )*3+2);
            
            // B
            new_indices_array[(p*4+1)*3  ] = ((p*4+1)*3 );
            new_indices_array[(p*4+1)*3+1] = ((p*4+1)*3+1);
            new_indices_array[(p*4+1)*3+2] = ((p*4+1)*3+2);
            
            // C
            new_indices_array[(p*4+2)*3  ] = ((p*4+2)*3 );
            new_indices_array[(p*4+2)*3+1] = ((p*4+2)*3+1);
            new_indices_array[(p*4+2)*3+2] = ((p*4+2)*3+2);
            
            // MIDDLE
            new_indices_array[(p*4+3)*3  ] = ((p*4+3)*3 );
            new_indices_array[(p*4+3)*3+1] = ((p*4+3)*3+1);
            new_indices_array[(p*4+3)*3+2] = ((p*4+3)*3+2);
        }

        vertices_amount = polygons_amount*4*3;
        free(vertices_position_arr);
        vertices_position_arr = new_vertices_position_arr;
        
        polygons_amount = polygons_amount*4;
        free(indices_array);
        indices_array = new_indices_array;
    }

    /*
    for (uint32_t i = 0; i < vertices_amount; i++){
        printf("%u:\t%f\t%f\t%f\n", i,
            vertices_position_arr[i*3  ],
            vertices_position_arr[i*3+1],
            vertices_position_arr[i*3+2]
        );
    }
    printf("\n");
    for (uint32_t i = 0; i < polygons_amount; i++){
        printf("%u:\t%u\t%u\t%u\n", i,
            indices_array[i*3  ],
            indices_array[i*3+1],
            indices_array[i*3+2]
        );
    }
    */
    
    for (uint32_t i = 0; i < vertices_amount; i++){ // normalise all vertices
        float len = sqrt(
            vertices_position_arr[i*3  ]*vertices_position_arr[i*3  ] +
            vertices_position_arr[i*3+1]*vertices_position_arr[i*3+1] +
            vertices_position_arr[i*3+2]*vertices_position_arr[i*3+2]
        );
        vertices_position_arr[i*3  ] /= len;
        vertices_position_arr[i*3+1] /= len;
        vertices_position_arr[i*3+2] /= len;
    }

    vbo_data_t vbo_datas_arr[1] = {
        {
            .data_arr_size = sizeof(float)*vertices_amount*3,
            .data_arr = (void*)vertices_position_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        }
    };

    mesh_t* result = generate_mesh(vbo_datas_arr, 1, indices_array, polygons_amount*3, 0);

    free(vertices_position_arr);
    free(indices_array);

    return result;
}

mesh_t* mesh_from_wavefront_obj_ext(const char* obj_file_path, quat_vec_vec_t transform_qvv, uint8_t unbinded) {
    char* obj_str;
    uint64_t file_length = load_file_contents(&obj_str,obj_file_path);
    if (obj_str == NULL) {
        return NULL;
    }

    mesh_t* mesh = NULL;

    uint32_t vertices_count = 0;
    uint32_t normals_count = 0;
    uint32_t uvs_count = 0;
    uint32_t faces_count = 0;
    uint32_t unique_vertices_count = 0;
    uint32_t polygons_count = 0; // triangles

    int64_t vertex_index;
    int64_t normal_index;
    int64_t uv_index;
    int64_t unique_vertex_index;
    int64_t polygon_index;
    int64_t face_first_unique_vertex_index;

    char last_char = '\0';
    // 0 = irrelevent
    // 1 = vertex
    // 2 = normal
    // 3 = uv
    // 4 = face
    uint8_t line_type = 0;
    uint32_t line_space_count = 0;
    uint32_t between_spaces_slash_count;

    uint64_t i;

    vec3_t vec3;

    vec3_t* vertices;
    vec3_t* normals;
    vec2_t* uvs;
    uint32_t* indices_array;
    float* vertices_position_arr;
    float* vertices_texcoord_arr;
    float* vertices_normal_arr;

    int32_t num;


    // count
    for (i = 0; i < file_length; i++) {
        if (last_char == '\n') {
            line_space_count = 0;

            // line type after newline
            if (obj_str[i] == 'v') {
                if (obj_str[i+1] == 'n') {
                    line_type = 2;
                    i += 2;
                    normals_count += 1;
                }else if (obj_str[i+1] == 't') {
                    line_type = 3;
                    i += 2;
                    uvs_count += 1;
                }else if (obj_str[i+1] == ' ') {
                    line_type = 1;
                    i += 1;
                    vertices_count += 1;
                }else {
                    printf("wierd read at obj file load\n");
                    line_type = 0;
                }
            }else if (obj_str[i] == 'f') {
                line_type = 4;
                i += 1;
                faces_count += 1;
                polygons_count += 1;
            }else {
                line_type = 0;
            }
        }

        // multipile polyons at 1 face
        if (line_type == 4) {
            if (obj_str[i] == ' ') {
                line_space_count += 1;
                unique_vertices_count += 1;
                if (line_space_count > 3) {
                    polygons_count += 1;
                }
            }
        }

        last_char = obj_str[i];
    }


    vertices = malloc(sizeof(vec3_t)*vertices_count);
    normals = malloc(sizeof(vec3_t)*normals_count);
    uvs = malloc(sizeof(vec2_t)*uvs_count);
    // final
    indices_array = malloc(sizeof(uint32_t)*polygons_count*3);
    vertices_position_arr = malloc(sizeof(float)*unique_vertices_count*3);
    vertices_texcoord_arr = malloc(sizeof(float)*unique_vertices_count*2);
    vertices_normal_arr = malloc(sizeof(float)*unique_vertices_count*3);
    
    if (
        vertices == NULL ||
        normals == NULL ||
        uvs == NULL ||
        vertices_position_arr == NULL ||
        indices_array == NULL ||
        vertices_texcoord_arr == NULL ||
        vertices_normal_arr == NULL
    ) {
        goto clean_and_return;
    }


    // set array
    vertex_index = -1;
    normal_index = -1;
    uv_index = -1;
    unique_vertex_index = -1;
    polygon_index = -1;
    face_first_unique_vertex_index = -1;
    
    line_type = 0;
    line_space_count = 0;
    between_spaces_slash_count = 0;
    
    for (i = 0; i < file_length; i++) {
        if (last_char == '\n') {
            line_space_count = 0;

            // line type after newline
            if (obj_str[i] == 'v') {
                if (obj_str[i+1] == 'n') {
                    line_type = 2;
                    i += 2;
                    normal_index += 1;
                }else if (obj_str[i+1] == 't') {
                    line_type = 3;
                    i += 2;
                    uv_index += 1;
                }else if (obj_str[i+1] == ' ') {
                    line_type = 1;
                    i += 1;
                    vertex_index += 1;
                }else {
                    printf("wierd read at obj file load\n");
                    line_type = 0;
                }
            }else if (obj_str[i] == 'f') {
                line_type = 4;
                i += 1;
                polygon_index += 1;
            }else {
                line_type = 0;
            }
        }

        if (line_type == 1) {
            if (obj_str[i] == ' ') {
                if (line_space_count == 0) {
                    vertices[vertex_index].x = atof(obj_str+i);
                }
                if (line_space_count == 1) {
                    vertices[vertex_index].y = atof(obj_str+i);
                }
                if (line_space_count == 2) {
                    vertices[vertex_index].z = atof(obj_str+i);
                }
                line_space_count += 1;
            }
        }else if (line_type == 2) {
            if (obj_str[i] == ' ') {
                if (line_space_count == 0) {
                    normals[normal_index].x = atof(obj_str+i);
                }
                if (line_space_count == 1) {
                    normals[normal_index].y = atof(obj_str+i);
                }
                if (line_space_count == 2) {
                    normals[normal_index].z = atof(obj_str+i);
                }
                line_space_count += 1;
            }
        }else if (line_type == 3) {
            if (obj_str[i] == ' ') {
                if (line_space_count == 0) {
                    uvs[uv_index].x = atof(obj_str+i);
                }
                if (line_space_count == 1) {
                    uvs[uv_index].y = atof(obj_str+i);
                }
                line_space_count += 1;
            }
        }if (line_type == 4) {
            if (obj_str[i] == ' ') {
                between_spaces_slash_count = 0;

                // append unique vertex
                unique_vertex_index += 1;
                num = atoi(obj_str+i);
                vertices_position_arr[unique_vertex_index*3    ] = vertices[num-1].x;
                vertices_position_arr[unique_vertex_index*3 + 1] = vertices[num-1].y;
                vertices_position_arr[unique_vertex_index*3 + 2] = vertices[num-1].z;
                
                if (line_space_count == 0) {
                    face_first_unique_vertex_index = unique_vertex_index;
                }
                
                if (line_space_count <= 2) {
                    indices_array[polygon_index*3 + line_space_count] = unique_vertex_index
                                                    /* fix indices order for face culling */ + (-line_space_count+1)*2;
                }else {
                    // multipile polyons at 1 face
                    // add an entire polygon
                    polygon_index += 1;
                    
                    indices_array[polygon_index*3    ] = unique_vertex_index;
                    indices_array[polygon_index*3 + 1] = unique_vertex_index-1;
                    indices_array[polygon_index*3 + 2] = face_first_unique_vertex_index;
                }
                
                line_space_count += 1;
            }else if (obj_str[i] == '/') {
                between_spaces_slash_count += 1;

                num = atoi(obj_str+i+1);

                if (between_spaces_slash_count == 1) {
                    vertices_texcoord_arr[unique_vertex_index*2    ] = uvs[num-1].x;
                    vertices_texcoord_arr[unique_vertex_index*2 + 1] = uvs[num-1].y;
                }else {
                    vertices_normal_arr[unique_vertex_index*3    ] = normals[num-1].x;
                    vertices_normal_arr[unique_vertex_index*3 + 1] = normals[num-1].y;
                    vertices_normal_arr[unique_vertex_index*3 + 2] = normals[num-1].z;
                }
                
            }
        }

        last_char = obj_str[i];
    }

    // transform
    for (i = 0; i < unique_vertices_count; i++) {
        vec3 = (vec3_t){
            .x = vertices_position_arr[i*3    ],
            .y = vertices_position_arr[i*3 + 1],
            .z = vertices_position_arr[i*3 + 2]
        };
        vec3 = vec_scale_rotate_translate(vec3, transform_qvv);
        vertices_position_arr[i*3    ] = vec3.x;
        vertices_position_arr[i*3 + 1] = vec3.y;
        vertices_position_arr[i*3 + 2] = vec3.z;
    }
    for (i = 0; i < unique_vertices_count; i++) {
        vec3 = (vec3_t){
            .x = vertices_normal_arr[i*3    ],
            .y = vertices_normal_arr[i*3 + 1],
            .z = vertices_normal_arr[i*3 + 2]
        };
        vec3 = rotate_vector(vec3, transform_qvv.rot);
        vertices_normal_arr[i*3    ] = vec3.x;
        vertices_normal_arr[i*3 + 1] = vec3.y;
        vertices_normal_arr[i*3 + 2] = vec3.z;
    }

    // <debug>
        #if 0
        printf(
            "data from '%s':\n  vertices count: %u\n  normals count: %u\n  uvs count: %u\n  unique_vertices_count: %u\n  faces count: %u\n  polygons count: %u\n",
            obj_file_path,
            vertices_count,
            normals_count,
            uvs_count,
            unique_vertices_count,
            faces_count,
            polygons_count
        );
        printf(
            "  counts:\n    vertex_index: %llu\n    normal_index: %llu\n    uv_index: %llu\n    unique_vertex_index: %llu\n    polygon_index: %llu\n",
            vertex_index,
            normal_index,
            uv_index,
            unique_vertex_index,
            polygon_index
        );
        printf("  vertices:[\n");
        for (uint64_t k = 0; k < vertices_count; k++) {
            printf("    (%f, %f, %f),\n", vertices[k].x, vertices[k].y, vertices[k].z);
        }
        printf("  ]\n  normals:[\n");
        for (uint64_t k = 0; k < normals_count; k++) {
            printf("    (%f, %f, %f),\n", normals[k].x, normals[k].y, normals[k].z);
        }
        printf("  ]\n  uvs:[\n");
        for (uint64_t k = 0; k < uvs_count; k++) {
            printf("    (%f, %f),\n", uvs[k].x, uvs[k].y);
        }
        printf("  ]\n  indices_array:[\n");
        for (uint64_t k = 0; k < polygons_count; k++) {
            printf("    (%u, %u, %u),\n", indices_array[k*3], indices_array[k*3+1], indices_array[k*3+2]);
        }
        printf("  ]\n  vertices_position_arr:[\n");
        for (uint64_t k = 0; k < unique_vertices_count; k++) {
            printf("    (%f, %f, %f),\n", vertices_position_arr[k*3], vertices_position_arr[k*3+1], vertices_position_arr[k*3+2]);
        }
        printf("  ]\n  vertices_texcoord_arr:[\n");
        for (uint64_t k = 0; k < unique_vertices_count; k++) {
            printf("    (%f, %f),\n", vertices_texcoord_arr[k*2], vertices_texcoord_arr[k*2+1]);
        }
        printf("  ]\n  vertices_normal_arr:[\n");
        for (uint64_t k = 0; k < unique_vertices_count; k++) {
            printf("    (%f, %f, %f),\n", vertices_normal_arr[k*3], vertices_normal_arr[k*3+1], vertices_normal_arr[k*3+2]);
        }
        printf("  ]\n\n");
        #endif
    // </debug>

    vbo_data_t vbo_datas_arr[3] = {
        {
            .data_arr_size = sizeof(float)*unique_vertices_count*3,
            .data_arr = (void*)vertices_position_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(float)*unique_vertices_count*2,
            .data_arr = (void*)vertices_texcoord_arr,
            .size = 2,
            .type = GL_FLOAT,
            .stride = 2*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(float)*unique_vertices_count*3,
            .data_arr = (void*)vertices_normal_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        }
    };

    mesh = generate_mesh(vbo_datas_arr, 3, indices_array, polygons_count*3, unbinded);

    goto clean_and_return;
    clean_and_return: {
        if (obj_str != NULL)               {free(obj_str);}
        if (vertices != NULL)              {free(vertices);}
        if (normals != NULL)               {free(normals);}
        if (uvs != NULL)                   {free(uvs);}
        if (indices_array != NULL)         {free(indices_array);}
        if (vertices_position_arr != NULL) {free(vertices_position_arr);}
        if (vertices_texcoord_arr != NULL) {free(vertices_texcoord_arr);}
        if (vertices_normal_arr != NULL)   {free(vertices_normal_arr);}
        
        return mesh;
    }
}
mesh_t* mesh_from_wavefront_obj(const char* obj_file_path, uint8_t unbinded) {
    quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
        .rot = (quat_t){
            .w = 1,
            .x = 0,
            .y = 0,
            .z = 0
        },
        .pos = (vec3_t){
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .scale = (vec3_t){
            .x = 1,
            .y = 1,
            .z = 1,
        }
    };
    return mesh_from_wavefront_obj_ext(obj_file_path, transform_qvv, unbinded);
}

void static mesh_from_collada_dae_joint_hierarchy(char* dae_str, mesh_t* mesh) {
    uint32_t i;
    int64_t str_i;
    int64_t dstr_i1;
    int64_t dstr_i2;
    uint32_t current_parent_joint;
    uint32_t current_joint;
    int32_t joint_depth;
    char* name;

    // find first joint
    str_i = str_find_substr(dae_str, "<node");
    while (str_find_substr(&(dae_str[str_i]), "type=\"NODE\">") < str_find_substr(&(dae_str[str_i]), "type=\"JOINT\">")) {
        str_i += str_find_substr(&(dae_str[str_i+1]), "<node");
    }
    
    joint_depth = -1;
    while (1) {
        dstr_i1 = str_find_substr(&(dae_str[str_i]), "</node>");
        dstr_i2 = str_find_substr(&(dae_str[str_i]), "<node");
        if (dstr_i1 < dstr_i2) {
            str_i += dstr_i1;
            str_i += str_find_substr(&(dae_str[str_i]), ">");
            // go back with current_joint and current_parent_joint
            current_joint = current_parent_joint;
            current_parent_joint = mesh->joints[current_parent_joint].parent;
            joint_depth -= 1;
            if (joint_depth < 0) {
                break;
            }
            continue;
        }

        // set str_i to the start of the "<node..." string
        str_i += dstr_i2;
        joint_depth += 1;

        // set current_parent_joint to the last joint
        current_parent_joint = current_joint;

        // find joint name
        str_i += str_find_substr(&(dae_str[str_i]), "name=\"");
        str_i += strlen("name=\"");
        dstr_i1 = str_find_substr(&(dae_str[str_i]), "\"");
        name = malloc(sizeof(char)*(dstr_i1+1));
        memcpy(name, &(dae_str[str_i]), dstr_i1);
        name[dstr_i1] = '\0';
        // find joint index from name
        for (i = 0; i < mesh->joints_amount; i++) {
            if (strcmp(name, mesh->joints[i].name) == 0) {
                current_joint = i;
                break;
            }
        }
        free(name);

        if (joint_depth == 0) {
            current_parent_joint = current_joint;
        }

        // set parent
        mesh->joints[current_joint].parent = current_parent_joint;
        
        // go to end of joint row
        str_i += str_find_substr(&(dae_str[str_i]), ">");
    }

}
mesh_t* mesh_from_collada_dae_ext(const char* dae_file_path, quat_vec_vec_t transform_qvv, uint8_t unbinded) {
    char* dae_str;
    uint64_t file_length = load_file_contents(&dae_str,dae_file_path);
    if (dae_str == NULL) {
        return NULL;
    }

    mesh_t* mesh;
    int64_t str_i;
    int64_t dstr_i;

    uint64_t vertices_positions_array_length;
    float* vertices_positions_array;
    uint64_t vertices_normals_array_length;
    float* vertices_normals_array;
    uint64_t vertices_texcoords_array_length;
    float* vertices_texcoords_array;
    uint64_t vertices_joint_ids_array_length;
    int32_t* vertices_joint_ids_array;
    uint64_t vertices_joint_wheights_array_length;
    float* vertices_joint_wheights_array;
    
    uint64_t joints_data_count_array_length;
    int32_t* joints_data_count_array;
    uint64_t joints_data_array_length;
    int32_t* joints_data_array;
    uint64_t joint_wheights_array_length;
    float* joint_wheights_array;

    int32_t i;
    int32_t j;
    uint64_t triangles_count;
    uint64_t current_triangles_count;
    uint64_t current_triangle_index;
    int32_t* triangles_data_arr;
    int32_t vertex_i;
    int32_t normal_i;
    int32_t texcoord_i;
    int32_t joint_data_count;
    uint64_t joint_data_index;
    float joint_data_wheight_max1;
    int64_t joint_data_wheight_max1_index;
    float joint_data_wheight_max2;
    int64_t joint_data_wheight_max2_index;
    float joint_data_wheight_max3;
    int64_t joint_data_wheight_max3_index;
    float joint_data_wheight_tmp;
    int64_t joint_data_wheight_tmp_index;
    uint64_t indice_index;
    float vertex_joint_wheights_total;
    vec3_t vec3;

    uint32_t* indices_array;
    float* vbo_vertices_position_arr;
    float* vbo_vertices_texcoord_arr;
    float* vbo_vertices_normal_arr;
    int32_t* vbo_vertices_joint_id_arr;
    float* vbo_vertices_joint_wheight_arr;

    uint64_t joints_inverse_bind_matrices_array_length;
    float* joints_inverse_bind_matrices_array;

    mat4_t transform_mat = mat4_from_quat_vec_vec(transform_qvv);


    // vertex positions array
    str_i = str_find_substr(dae_str, "positions-array\" count=\"");
    str_i += strlen("positions-array\" count=\"");
    vertices_positions_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), "\">");
    str_i += strlen("\">");
    vertices_positions_array = str_to_float_array(&(dae_str[str_i]), vertices_positions_array_length);
    if (vertices_positions_array == NULL){
        goto clean_and_return;
    }
    
    // vertex normals array
    str_i = str_find_substr(dae_str, "normals-array\" count=\"");
    str_i += strlen("normals-array\" count=\"");
    vertices_normals_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), "\">");
    str_i += strlen("\">");
    vertices_normals_array = str_to_float_array(&(dae_str[str_i]), vertices_normals_array_length);
    if (vertices_normals_array == NULL){
        goto clean_and_return;
    }
    
    // vertex texcoords array
    str_i = str_find_substr(dae_str, "map-0-array\" count=\"");
    str_i += strlen("map-0-array\" count=\"");
    vertices_texcoords_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), "\">");
    str_i += strlen("\">");
    vertices_texcoords_array = str_to_float_array(&(dae_str[str_i]), vertices_texcoords_array_length);
    if (vertices_texcoords_array == NULL){
        goto clean_and_return;
    }
    
    // vertex joint wheights and vertex joint ids arrays
    // joint wheights array
    str_i = str_find_substr(dae_str, "skin-weights-array\" count=\"");
    str_i += strlen("skin-weights-array\" count=\"");
    joint_wheights_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), "\">");
    str_i += strlen("\">");
    joint_wheights_array = str_to_float_array(&(dae_str[str_i]), joint_wheights_array_length);
    if (joint_wheights_array == NULL){
        goto clean_and_return;
    }
    // joints data count array
    str_i = str_find_substr(dae_str, "<vertex_weights count=\"");
    str_i += strlen("<vertex_weights count=\"");
    joints_data_count_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), "<vcount>");
    str_i += strlen("<vcount>");
    joints_data_count_array = str_to_int_array(&(dae_str[str_i]), joints_data_count_array_length);
    if (joints_data_count_array == NULL){
        goto clean_and_return;
    }
    // joints data array
    joints_data_array_length = 0;
    for (i = 0; i < joints_data_count_array_length; i++) {
        joints_data_array_length += joints_data_count_array[i];
    }
    joints_data_array_length *= 2;
    str_i += str_find_substr(&(dae_str[str_i]), "<v>");
    str_i += strlen("<v>");
    joints_data_array = str_to_int_array(&(dae_str[str_i]), joints_data_array_length);
    if (joints_data_array == NULL){
        goto clean_and_return;
    }
    // vertex joint wheights and vertex joint ids arrays
    vertices_joint_ids_array = malloc(sizeof(int32_t)*joints_data_count_array_length*3);
    vertices_joint_wheights_array = malloc(sizeof(float)*joints_data_count_array_length*3);
    if (vertices_joint_ids_array == NULL || vertices_joint_wheights_array == NULL){
        goto clean_and_return;
    }
    joint_data_index = 0;
    for (i = 0; i < joints_data_count_array_length; i++) {
        joint_data_count = joints_data_count_array[i];
        if (joint_data_count < 3) {
            for (j = 0; j < 3; j++) {
                if (j < joint_data_count) {
                    vertices_joint_ids_array[i*3 + j]      = joints_data_array[joint_data_index*2];
                    vertices_joint_wheights_array[i*3 + j] = joint_wheights_array[joints_data_array[joint_data_index*2 + 1]];
                    joint_data_index += 1;
                }else {
                    vertices_joint_ids_array[i*3 + j]      = 0;
                    vertices_joint_wheights_array[i*3 + j] = 0;
                }
            }
        }else {
            // initial values
            joint_data_wheight_max1_index = -1;
            joint_data_wheight_max1 = 0;
            joint_data_wheight_max2_index = -1;
            joint_data_wheight_max2 = 0;
            joint_data_wheight_max3_index = -1;
            joint_data_wheight_max3 = 0;


            // find 3 biggest wheight values
            for (j = 0; j < joint_data_count; j++) {
                joint_data_wheight_tmp_index = joint_data_index + j;
                joint_data_wheight_tmp = joint_wheights_array[joints_data_array[joint_data_wheight_tmp_index*2 + 1]];

                if (joint_data_wheight_tmp > joint_data_wheight_max3) {
                    joint_data_wheight_max3 = joint_data_wheight_tmp;
                    joint_data_wheight_max3_index = joint_data_wheight_tmp_index;
                    
                    if (joint_data_wheight_max3 > joint_data_wheight_max2) {
                        joint_data_wheight_tmp        = joint_data_wheight_max2;
                        joint_data_wheight_tmp_index  = joint_data_wheight_max2_index;
                        joint_data_wheight_max2       = joint_data_wheight_max3;
                        joint_data_wheight_max2_index = joint_data_wheight_max3_index;
                        joint_data_wheight_max3       = joint_data_wheight_tmp;
                        joint_data_wheight_max3_index = joint_data_wheight_tmp_index;

                        if (joint_data_wheight_max2 > joint_data_wheight_max1) {
                            joint_data_wheight_tmp        = joint_data_wheight_max1;
                            joint_data_wheight_tmp_index  = joint_data_wheight_max1_index;
                            joint_data_wheight_max1       = joint_data_wheight_max2;
                            joint_data_wheight_max1_index = joint_data_wheight_max2_index;
                            joint_data_wheight_max2       = joint_data_wheight_tmp;
                            joint_data_wheight_max2_index = joint_data_wheight_tmp_index;
                        }
                    }
                }
            }

            // set data
            if (joint_data_wheight_max1_index >= 0) {
                vertices_joint_ids_array[i*3    ]      = joints_data_array[joint_data_wheight_max1_index*2];
                vertices_joint_wheights_array[i*3    ] = joint_wheights_array[joints_data_array[joint_data_wheight_max1_index*2 + 1]];
            }else {
                vertices_joint_ids_array[i*3    ] = 0;
                vertices_joint_wheights_array[i*3    ] = 0;
            }
            if (joint_data_wheight_max2_index >= 0) {
                vertices_joint_ids_array[i*3 + 1]      = joints_data_array[joint_data_wheight_max2_index*2];
                vertices_joint_wheights_array[i*3 + 1] = joint_wheights_array[joints_data_array[joint_data_wheight_max2_index*2 + 1]];
            }else {
                vertices_joint_ids_array[i*3 + 1] = 0;
                vertices_joint_wheights_array[i*3 + 1] = 0;
            }
            if (joint_data_wheight_max3_index >= 0) {
                vertices_joint_ids_array[i*3 + 2]      = joints_data_array[joint_data_wheight_max3_index*2];
                vertices_joint_wheights_array[i*3 + 2] = joint_wheights_array[joints_data_array[joint_data_wheight_max3_index*2 + 1]];
            }else {
                vertices_joint_ids_array[i*3 + 2] = 0;
                vertices_joint_wheights_array[i*3 + 2] = 0;
            }

            joint_data_index += joint_data_count;
        }

        vertex_joint_wheights_total = vertices_joint_wheights_array[i*3] + vertices_joint_wheights_array[i*3 + 1] + vertices_joint_wheights_array[i*3 + 2];
        if (vertex_joint_wheights_total != 0) {
            vertices_joint_wheights_array[i*3    ] /= vertex_joint_wheights_total;
            vertices_joint_wheights_array[i*3 + 1] /= vertex_joint_wheights_total;
            vertices_joint_wheights_array[i*3 + 2] /= vertex_joint_wheights_total;
        }
    }
    

    // find triangles_count
    triangles_count = 0;
    str_i = 0;
    while (1) {
        dstr_i = str_find_substr(&(dae_str[str_i]), "<triangles");
        str_i += dstr_i;
        if (dstr_i == -1) {
            break;
        }
        str_i += str_find_substr(&(dae_str[str_i]), "count=\"");
        str_i += strlen("count=\"");
        triangles_count += atoi(&(dae_str[str_i]));
    }

    // set all values
    indices_array = malloc(sizeof(uint32_t)*triangles_count*3);
    vbo_vertices_position_arr = malloc(sizeof(float)*triangles_count*3*3);
    vbo_vertices_texcoord_arr = malloc(sizeof(float)*triangles_count*3*2);
    vbo_vertices_normal_arr = malloc(sizeof(float)*triangles_count*3*3);
    vbo_vertices_joint_id_arr = malloc(sizeof(int32_t)*triangles_count*3*3);
    vbo_vertices_joint_wheight_arr = malloc(sizeof(float)*triangles_count*3*3);
    if (
        indices_array == NULL ||
        vbo_vertices_position_arr == NULL ||
        vbo_vertices_texcoord_arr == NULL ||
        vbo_vertices_normal_arr == NULL ||
        vbo_vertices_joint_id_arr == NULL ||
        vbo_vertices_joint_wheight_arr == NULL
    ) {
        goto clean_and_return;
    }

    str_i = 0;
    indice_index = 0;
    while (1) {
        dstr_i = str_find_substr(&(dae_str[str_i]), "<triangles");
        str_i += dstr_i;
        if (dstr_i == -1) {
            break;
        }
        str_i += str_find_substr(&(dae_str[str_i]), "count=\"");
        str_i += strlen("count=\"");
        current_triangles_count = atoi(&(dae_str[str_i]));
        str_i += str_find_substr(&(dae_str[str_i]), "<p>");
        str_i += strlen("<p>");
        triangles_data_arr = str_to_int_array(&(dae_str[str_i]), current_triangles_count*3*3);
        if (triangles_data_arr == NULL) {
            goto clean_and_return;
        }

        for (current_triangle_index = 0; current_triangle_index < current_triangles_count; current_triangle_index++) {
            for (i = 0; i < 3; i++) {
                vertex_i   = triangles_data_arr[current_triangle_index*3*3 + i*3    ];
                normal_i   = triangles_data_arr[current_triangle_index*3*3 + i*3 + 1];
                texcoord_i = triangles_data_arr[current_triangle_index*3*3 + i*3 + 2];
                
                vec3 = (vec3_t){
                    .x = vertices_positions_array[vertex_i*3    ],
                    .y = vertices_positions_array[vertex_i*3 + 1],
                    .z = vertices_positions_array[vertex_i*3 + 2]
                };
                vec3 = vec_scale_rotate_translate(vec3, transform_qvv);
                vbo_vertices_position_arr[(indice_index+i)*3    ] = vec3.x;
                vbo_vertices_position_arr[(indice_index+i)*3 + 1] = vec3.y;
                vbo_vertices_position_arr[(indice_index+i)*3 + 2] = vec3.z;
                
                vec3 = (vec3_t){
                    .x = vertices_normals_array[normal_i*3    ],
                    .y = vertices_normals_array[normal_i*3 + 1],
                    .z = vertices_normals_array[normal_i*3 + 2]
                };
                vec3 = rotate_vector(vec3, transform_qvv.rot);
                vbo_vertices_normal_arr[  (indice_index+i)*3    ] = vec3.x;
                vbo_vertices_normal_arr[  (indice_index+i)*3 + 1] = vec3.y;
                vbo_vertices_normal_arr[  (indice_index+i)*3 + 2] = vec3.z;
                
                vbo_vertices_texcoord_arr[(indice_index+i)*2    ] = vertices_texcoords_array[texcoord_i*2    ];
                vbo_vertices_texcoord_arr[(indice_index+i)*2 + 1] = vertices_texcoords_array[texcoord_i*2 + 1];
                
                vbo_vertices_joint_id_arr[(indice_index+i)*3    ]      = vertices_joint_ids_array[vertex_i*3    ];
                vbo_vertices_joint_id_arr[(indice_index+i)*3 + 1]      = vertices_joint_ids_array[vertex_i*3 + 1];
                vbo_vertices_joint_id_arr[(indice_index+i)*3 + 2]      = vertices_joint_ids_array[vertex_i*3 + 2];
                vbo_vertices_joint_wheight_arr[(indice_index+i)*3    ] = vertices_joint_wheights_array[vertex_i*3    ];
                vbo_vertices_joint_wheight_arr[(indice_index+i)*3 + 1] = vertices_joint_wheights_array[vertex_i*3 + 1];
                vbo_vertices_joint_wheight_arr[(indice_index+i)*3 + 2] = vertices_joint_wheights_array[vertex_i*3 + 2];
            }
            
            // // normal cull face
            // indices_array[indice_index    ] = indice_index;
            // indices_array[indice_index + 1] = indice_index + 1;
            // indices_array[indice_index + 2] = indice_index + 2;
            
            // reverse cull face
            indices_array[indice_index    ] = indice_index + 1;
            indices_array[indice_index + 1] = indice_index;
            indices_array[indice_index + 2] = indice_index + 2;

            indice_index += 3;
        }
    }

    
    vbo_data_t vbo_datas_arr[5] = {
        {
            .data_arr_size = sizeof(float)*triangles_count*3*3,
            .data_arr = (void*)vbo_vertices_position_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(float)*triangles_count*3*2,
            .data_arr = (void*)vbo_vertices_texcoord_arr,
            .size = 2,
            .type = GL_FLOAT,
            .stride = 2*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(float)*triangles_count*3*3,
            .data_arr = (void*)vbo_vertices_normal_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(int32_t)*triangles_count*3*3,
            .data_arr = (void*)vbo_vertices_joint_id_arr,
            .size = 3,
            .type = GL_INT,
            .stride = 3*sizeof(int32_t),
            .divisor = 0
        },
        {
            .data_arr_size = sizeof(float)*triangles_count*3*3,
            .data_arr = (void*)vbo_vertices_joint_wheight_arr,
            .size = 3,
            .type = GL_FLOAT,
            .stride = 3*sizeof(float),
            .divisor = 0
        }
    };

    mesh = generate_mesh(vbo_datas_arr, 5, indices_array, triangles_count*3, unbinded);


    // mesh->joints
    str_i = str_find_substr(dae_str, "skin-joints-array\" count=\"");
    str_i += strlen("skin-joints-array\" count=\"");
    mesh->joints_amount = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), ">");
    str_i += strlen(">");
    mesh->joints = malloc(sizeof(joint_t)*mesh->joints_amount);
    if (mesh->joints == NULL) {
        // give up on joints :(
        goto clean_and_return;
    }
    // joint names
    for (i = 0; i < mesh->joints_amount; i++) {
        mesh->joints[i].index = i;

        dstr_i = int64min(str_find_substr(&(dae_str[str_i]), " "), str_find_substr(&(dae_str[str_i]), "</Name_array>"));
        mesh->joints[i].name = malloc(sizeof(char)*(dstr_i+1));
        if (mesh->joints[i].name != NULL) {
            memcpy(mesh->joints[i].name, &(dae_str[str_i]), dstr_i);
            mesh->joints[i].name[dstr_i] = '\0';
        }
        str_i += dstr_i+1;
    }
    // joint inverse bind matrices
    str_i = str_find_substr(dae_str, "skin-bind_poses-array\" count=\"");
    str_i += strlen("skin-bind_poses-array\" count=\"");
    joints_inverse_bind_matrices_array_length = atoi(&(dae_str[str_i]));
    str_i += str_find_substr(&(dae_str[str_i]), ">");
    str_i += strlen(">");
    joints_inverse_bind_matrices_array = str_to_float_array(&(dae_str[str_i]), joints_inverse_bind_matrices_array_length);
    if (joints_inverse_bind_matrices_array == NULL) {
        goto clean_and_return;
    }
    for (i = 0; i < mesh->joints_amount; i++) {
        memcpy(mesh->joints[i].inverse_bind_transform_mat.mat, &(joints_inverse_bind_matrices_array[i*16]), sizeof(float)*16);
    }
    // joint hierarchy
    mesh_from_collada_dae_joint_hierarchy(dae_str, mesh);

    mesh->pose_joint_transform_matrices = malloc(sizeof(float)*16*mesh->joints_amount);

    
    goto clean_and_return;
    clean_and_return: {
        if (dae_str != NULL)                            {free(dae_str);}
        if (vertices_positions_array != NULL)           {free(vertices_positions_array);}
        if (vertices_normals_array != NULL)             {free(vertices_normals_array);}
        if (vertices_texcoords_array != NULL)           {free(vertices_texcoords_array);}
        if (vertices_joint_ids_array != NULL)           {free(vertices_joint_ids_array);}
        if (vertices_joint_wheights_array != NULL)      {free(vertices_joint_wheights_array);}
        if (joints_data_count_array != NULL)            {free(joints_data_count_array);}
        if (joints_data_array != NULL)                  {free(joints_data_array);}
        if (joint_wheights_array != NULL)               {free(joint_wheights_array);}
        if (indices_array != NULL)                      {free(indices_array);}
        if (vbo_vertices_position_arr != NULL)          {free(vbo_vertices_position_arr);}
        if (vbo_vertices_texcoord_arr != NULL)          {free(vbo_vertices_texcoord_arr);}
        if (vbo_vertices_normal_arr != NULL)            {free(vbo_vertices_normal_arr);}
        if (vbo_vertices_joint_id_arr != NULL)          {free(vbo_vertices_joint_id_arr);}
        if (vbo_vertices_joint_wheight_arr != NULL)     {free(vbo_vertices_joint_wheight_arr);}
        if (triangles_data_arr != NULL)                 {free(triangles_data_arr);}
        if (joints_inverse_bind_matrices_array != NULL) {free(joints_inverse_bind_matrices_array);}
        
        return mesh;
    }
}
mesh_t* mesh_from_collada_dae(const char* dae_file_path, uint8_t unbinded) {
    quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
        .rot = (quat_t){
            .w = 1,
            .x = 0,
            .y = 0,
            .z = 0
        },
        .pos = (vec3_t){
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .scale = (vec3_t){
            .x = 1,
            .y = 1,
            .z = 1,
        }
    };
    return mesh_from_collada_dae_ext(dae_file_path, transform_qvv, unbinded);
}

animation_t* animation_from_collada_dae_ext(const char* dae_file_path, joint_t* joints, uint32_t joints_amount, quat_vec_vec_t transform_qvv) {
    if (animations_amount >= ANIMATIONS_MAX_AMOUNT) {
        return NULL;
    }

    char* dae_str;
    uint64_t file_length = load_file_contents(&dae_str,dae_file_path);
    if (dae_str == NULL) {
        return NULL;
    }
    
    animation_t* anim;

    uint32_t i;

    int64_t str_i;
    int64_t dstr_i;

    char* name;
    int32_t joint_i;

    float* key_frames_time_stamp;

    uint32_t key_frames_transform_matrices_array_length;
    float* key_frames_transform_matrices_array;

    mat4_t transform_mat = mat4_from_quat_vec_vec(transform_qvv);

    
    anim = malloc(sizeof(animation_t));
    if (anim == NULL) {
        printf("malloc failed at animation generation.\n");
        free(dae_str);
        return NULL;
    }

    anim->joints_amount = joints_amount;
    anim->joints_key_frames = calloc(1,sizeof(joint_key_frame_t)*joints_amount);

    // for every joint
    str_i = 0;
    while (1) {
        // find joint name
        dstr_i = str_find_substr(&(dae_str[str_i]), "<float_array id=\"Armature_ArmatureAction_001_");
        if (dstr_i == -1) {
            break;
        }
        str_i += dstr_i;
        str_i += strlen("<float_array id=\"Armature_ArmatureAction_001_");
        dstr_i = str_find_substr(&(dae_str[str_i]), "_pose_matrix-input-array\" count=\"");
        name = malloc(sizeof(char)*(dstr_i+1));
        memcpy(name, &(dae_str[str_i]), dstr_i);
        name[dstr_i] = '\0';
        // find joint index from name
        joint_i = -1;
        for (i = 0; i < joints_amount; i++) {
            if (strcmp(name, joints[i].name) == 0) {
                joint_i = i;
                break;
            }
        }
        free(name);

        if (joint_i == -1) {
            printf("could not find joint \"%s\"\n", name);
            continue;
        }

        // find joints' keyframes amount
        str_i += dstr_i;
        str_i += strlen("_pose_matrix-input-array\" count=\"");
        anim->joints_key_frames[joint_i].key_frames_amount = atoi(&(dae_str[str_i]));
        anim->joints_key_frames[joint_i].key_frames = malloc(sizeof(key_frame_t)*anim->joints_key_frames[joint_i].key_frames_amount);

        str_i += str_find_substr(&(dae_str[str_i]), "\">");
        key_frames_time_stamp = str_to_float_array(&(dae_str[str_i]), anim->joints_key_frames[joint_i].key_frames_amount);

        for (i = 0; i < anim->joints_key_frames[joint_i].key_frames_amount; i++) {
            anim->joints_key_frames[joint_i].key_frames[i].time_stamp = key_frames_time_stamp[i];
        }

        free(key_frames_time_stamp);

        // find joints' transforms
        str_i += str_find_substr(&(dae_str[str_i]), "_pose_matrix-output-array\" count=\"");
        str_i += strlen("_pose_matrix-output-array\" count=\"");
        key_frames_transform_matrices_array_length = atoi(&(dae_str[str_i]));
        str_i += str_find_substr(&(dae_str[str_i]), "\">");
        str_i += strlen("\">");
        key_frames_transform_matrices_array = str_to_float_array(&(dae_str[str_i]), key_frames_transform_matrices_array_length);
        for (i = 0; i < anim->joints_key_frames[joint_i].key_frames_amount; i++) {
            memcpy(
                anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform.mat,
                &(key_frames_transform_matrices_array[i*16]), sizeof(float)*16
            );

            // apply transform
            if (joint_i == 0) {
                anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform = mat4_mul(
                    transform_mat,
                    anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform
                );
            }

            anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform_qvv = quat_vec_vec_from_mat4(anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform);
        }
        free(key_frames_transform_matrices_array);
    }

    // find local matrix for joints without animation
    for (joint_i = 0; joint_i < joints_amount; joint_i++) {
        if (anim->joints_key_frames[joint_i].key_frames_amount != 0) {
            continue;
        }

        str_i = str_find_substr(dae_str, "<visual_scene");
        str_i += str_find_substr(&(dae_str[str_i]), joints[joint_i].name);
        str_i += str_find_substr(&(dae_str[str_i]), "<matrix");
        str_i += str_find_substr(&(dae_str[str_i]), ">");
        str_i += strlen(">");
        key_frames_transform_matrices_array = str_to_float_array(&(dae_str[str_i]), 16);
        
        anim->joints_key_frames[joint_i].key_frames_amount = 1;
        anim->joints_key_frames[joint_i].key_frames = malloc(sizeof(key_frame_t)*anim->joints_key_frames[joint_i].key_frames_amount);

        memcpy(
            anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform.mat,
            key_frames_transform_matrices_array, sizeof(float)*16
        );
        
        // apply transform
        if (joint_i == 0) {
            anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform = mat4_mul(
                transform_mat,
                anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform
            );
        }
        
        anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform_qvv = quat_vec_vec_from_mat4(anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform);

        free(key_frames_transform_matrices_array);
    }

    free(dae_str);

    *((uint64_t*)&anim->animation_index) = animations_amount;
    animations_list[animations_amount] = anim;
    animations_amount += 1;
    
    return anim;
}
animation_t* animation_from_collada_dae(const char* dae_file_path, joint_t* joints, uint32_t joints_amount) {
    quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
        .rot = (quat_t){
            .w = 1,
            .x = 0,
            .y = 0,
            .z = 0
        },
        .pos = (vec3_t){
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .scale = (vec3_t){
            .x = 1,
            .y = 1,
            .z = 1,
        }
    };
    return animation_from_collada_dae_ext(dae_file_path, joints, joints_amount, transform_qvv);
}

quat_vec_vec_t static mix_joint_local_transform_qvvs(quat_vec_vec_t qvv1, quat_vec_vec_t qvv2, float mix) {
    return (quat_vec_vec_t){
        .rot = quat_slerp(qvv1.rot, qvv2.rot, mix),
        .pos = {
            .x = qvv1.pos.x*(1-mix) + qvv2.pos.x*(mix),
            .y = qvv1.pos.y*(1-mix) + qvv2.pos.y*(mix),
            .z = qvv1.pos.z*(1-mix) + qvv2.pos.z*(mix)
        },
        .scale = {
            .x = qvv1.scale.x*(1-mix) + qvv2.scale.x*(mix),
            .y = qvv1.scale.y*(1-mix) + qvv2.scale.y*(mix),
            .z = qvv1.scale.z*(1-mix) + qvv2.scale.z*(mix)
        }
    };
}
// translates keyframes to achive local transform qvv
quat_vec_vec_t static get_joint_local_transform_qvv_at_animation_time(animation_t* anim, uint32_t joint_i, float time_stamp) {
    joint_key_frame_t* joint_key_frame = &(anim->joints_key_frames[joint_i]);

    if (joint_key_frame->key_frames_amount == 0) {
        return (quat_vec_vec_t){
            .rot = {
                .w = 1,
                .x = 0,
                .y = 0,
                .z = 0
            },
            .pos = {
                .x = 0,
                .y = 0,
                .z = 0
            },
            .scale = {
                .x = 1,
                .y = 1,
                .z = 1
            }
        };
    }
    
    // under minimum time stamp
    if (joint_key_frame->key_frames_amount == 1 || time_stamp < joint_key_frame->key_frames[0].time_stamp) {
        return joint_key_frame->key_frames[0].joint_local_transform_qvv;
    }
    
    uint32_t key_frame_prev = 0;
    for (uint32_t i = 0; i < joint_key_frame->key_frames_amount; i++) {
        if (time_stamp < joint_key_frame->key_frames[i].time_stamp) {
            break;
        }
        key_frame_prev = i;
    }

    // over maximum time stamp
    if (key_frame_prev == joint_key_frame->key_frames_amount - 1) {
        return joint_key_frame->key_frames[joint_key_frame->key_frames_amount - 1].joint_local_transform_qvv;
    }

    float t_linear =
        (time_stamp - joint_key_frame->key_frames[key_frame_prev].time_stamp) /
        (joint_key_frame->key_frames[key_frame_prev + 1].time_stamp - joint_key_frame->key_frames[key_frame_prev].time_stamp);

    return mix_joint_local_transform_qvvs(
        joint_key_frame->key_frames[key_frame_prev].joint_local_transform_qvv,
        joint_key_frame->key_frames[key_frame_prev + 1].joint_local_transform_qvv,
        t_linear
    );
}
// mix: between 0 and 1; 0 = no effect; 1 = overides current animation
void pose_mesh_mix_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp, float mix) {
    if (mesh->joints_amount != anim->joints_amount) {
        return;
    }

    mix = fmax(fmin(mix, 1), 0); // clamp between 0 and 1

    uint32_t joint_i;
    joint_t* joint;

    for (joint_i = 0; joint_i < anim->joints_amount; joint_i++) {
        joint = &(mesh->joints[joint_i]);
        joint->local_transform_qvv = mix_joint_local_transform_qvvs(
            joint->local_transform_qvv,
            get_joint_local_transform_qvv_at_animation_time(anim, joint_i, time_stamp),
            mix
        );
    }
}
void pose_mesh_set_from_animation(mesh_t* mesh, animation_t* anim, float time_stamp) {
    pose_mesh_mix_from_animation(mesh, anim, time_stamp, 1);
}


void draw_mesh_instanced(mesh_t* mesh, uint32_t instance_count) {
    // if mesh has joints -> set all joint_transform_matrices to identity mat4
    if (mesh->joints_amount > 0) {
        float* joints_transform_matrices = malloc(sizeof(float)*16*mesh->joints_amount);
        if (joints_transform_matrices != NULL) {
            for (uint32_t joint_i = 0; joint_i < mesh->joints_amount; joint_i++) {
                joints_transform_matrices[joint_i*16    ] = 1;
                joints_transform_matrices[joint_i*16 + 1] = 0;
                joints_transform_matrices[joint_i*16 + 2] = 0;
                joints_transform_matrices[joint_i*16 + 3] = 0;
                
                joints_transform_matrices[joint_i*16 + 4] = 0;
                joints_transform_matrices[joint_i*16 + 5] = 1;
                joints_transform_matrices[joint_i*16 + 6] = 0;
                joints_transform_matrices[joint_i*16 + 7] = 0;
                
                joints_transform_matrices[joint_i*16 + 8] = 0;
                joints_transform_matrices[joint_i*16 + 9] = 0;
                joints_transform_matrices[joint_i*16 + 10] = 1;
                joints_transform_matrices[joint_i*16 + 11] = 0;
                
                joints_transform_matrices[joint_i*16 + 12] = 0;
                joints_transform_matrices[joint_i*16 + 13] = 0;
                joints_transform_matrices[joint_i*16 + 14] = 0;
                joints_transform_matrices[joint_i*16 + 15] = 1;
            }
            glUniformMatrix4fv(shaders_list[current_shader]->u_joint_matrices_loc, mesh->joints_amount, GL_FALSE, joints_transform_matrices);
            free(joints_transform_matrices);
        }
    }

    // draw mesh
    if (instance_count == 1) {
        // draw not instanced
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array);
    }else {
        glBindVertexArray(mesh->vao);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array, instance_count);
    }
}
void draw_mesh(mesh_t* mesh) {
    draw_mesh_instanced(mesh, 1);
}
void draw_mesh_posed_instanced(mesh_t* mesh, uint32_t instance_count) {
    uint32_t joint_i;
    joint_t* joint;
    mat4_t final_joint_transform_mat;

    // calculate pose from joints' local transform matrices
    for (joint_i = 0; joint_i < mesh->joints_amount; joint_i++) {
        joint = &(mesh->joints[joint_i]);
        if (joint->parent == joint_i) {
            joint->model_transform_mat = mat4_from_quat_vec_vec(joint->local_transform_qvv);
        }else {
            joint->model_transform_mat = mat4_mul(
                mesh->joints[joint->parent].model_transform_mat,
                mat4_from_quat_vec_vec(joint->local_transform_qvv)
            );
        }
        
        final_joint_transform_mat = mat4_mul(joint->model_transform_mat, joint->inverse_bind_transform_mat);

        memcpy(&(mesh->pose_joint_transform_matrices[joint_i*16]), final_joint_transform_mat.mat, sizeof(float)*16);
    }

    // set pose
    glUniformMatrix4fv(shaders_list[current_shader]->u_joint_matrices_loc, mesh->joints_amount, GL_FALSE, mesh->pose_joint_transform_matrices);
    
    // draw mesh
    if (instance_count == 1) {
        // draw not instanced
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array);
    }else {
        glBindVertexArray(mesh->vao);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array, instance_count);
    }
}
void draw_mesh_posed(mesh_t* mesh) {
    draw_mesh_posed_instanced(mesh, 1);
}

static void clean_mesh(mesh_t* mesh) {
    glDeleteBuffers(mesh->vbos_amount, mesh->vbos);
    glDeleteBuffers(1, &mesh->vao);
    free(mesh->vbos);
    free(mesh->indices_array);
    if (mesh->joints != NULL) {
        for (uint32_t i = 0; i < mesh->joints_amount; i++) {
            if (mesh->joints[i].name != NULL) {
                free(mesh->joints[i].name);
            }
        }
        free(mesh->joints);
    }
    if (mesh->pose_joint_transform_matrices != NULL) {
        free(mesh->pose_joint_transform_matrices);
    }
    free(mesh);
    return;
}
static void clean_animation(animation_t* anim) {
    for (uint32_t i = 0; i < anim->joints_amount; i++) {
        if (anim->joints_key_frames[i].key_frames != NULL) {
            free(anim->joints_key_frames[i].key_frames);
        }
    }
    free(anim->joints_key_frames);
    free(anim);
}

void destroy_mesh(mesh_t* mesh) {
    if (mesh->saved == 1) {
        #ifdef DEBUG_SOFT_MODE
        printf("called destroy a saved mesh! not doint that\n");
        return;
        #endif
    }

    meshes_amount -= 1;

    // move (meshes_list[meshes_amount]) to position (mesh->mesh_index) at (meshes_list)
    mesh_t* last_mesh = meshes_list[meshes_amount];
    *((uint64_t*)&last_mesh->mesh_index) = mesh->mesh_index;
    meshes_list[mesh->mesh_index] = last_mesh;

    clean_mesh(mesh);
}
void destroy_animation(animation_t* anim) {
    animations_amount -= 1;

    // move (animations_list[animations_amount]) to position (anim->animation_index) at (animations_list)
    animation_t* last_anim = animations_list[animations_amount];
    *((uint64_t*)&last_anim->animation_index) = anim->animation_index;
    animations_list[anim->animation_index] = last_anim;

    clean_animation(anim);
}

void clean_meshes() {
    printf("cleaning %u meshes\n", meshes_amount);
    for (uint64_t i = 0; i < meshes_amount; i++) clean_mesh(meshes_list[i]);
    meshes_amount = 0;
}
void clean_animations() {
    printf("cleaning %u animations\n", animations_amount);
    for (uint64_t i = 0; i < animations_amount; i++) clean_animation(animations_list[i]);
    animations_amount = 0;
}

void save_mesh_to_c_file(mesh_t* mesh, const char* name, const char* file_path) {
    if (mesh->unbinded == 0) return;

    FILE* fp;
    fp = fopen(file_path, "w");
    if (fp == NULL) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open file \"%s\" to save mesh \"%s\" to.\n", file_path, name);
        #endif
        return;
    }
    
    printf("saving mesh \"%s\" to file \"%s\"\n", name, file_path);

    fprintf(fp, "#include <stdint.h>\n");

    // mesh_vbo_data_arrX
    for (uint64_t i = 0; i < mesh->vbos_amount; i++) {
        fprintf(fp,
            ""      "static uint8_t %s_mesh_vbo_data_arr%llu[] = {"
            ,
            name,
            i
        );
        for (uint64_t j = 0; j < mesh->vbo_datas_arr[i].data_arr_size; j++) {
            if (j != 0) fprintf(fp,", ");
            if (j%20 == 0) fprintf(fp,"\n\t");
            fprintf(fp,
                "%hhu",
                ((uint8_t*)(mesh->vbo_datas_arr[i].data_arr))[j]
            );
        }
        fprintf(fp, "\n};\n");
    }

    // mesh_vbos
    fprintf(fp,
        ""      "static uint32_t %s_mesh_vbos[] = {"
        ,
        name
    );
    for (uint64_t i = 0; i < mesh->vbos_amount; i++) {
        if (i != 0) fprintf(fp, ", ");
        if (i%20 == 0) fprintf(fp,"\n\t");
        fprintf(fp, "0");
    }
    fprintf(fp,
        ""      "\n};\n"
    );

    // mesh_vbo_datas_arr
    fprintf(fp,
        ""      "static vbo_data_t %s_mesh_vbo_datas_arr[] = {\n"
        ,
        name
    );
    for (uint64_t i = 0; i < mesh->vbos_amount; i++) {
        if (i != 0) fprintf(fp,",\n");
        fprintf(fp,
            "\t"        "{\n"
            "\t\t"          ".data_arr_size = %u,\n"
            "\t\t"          ".data_arr = %s_mesh_vbo_data_arr%llu,\n"
            "\t\t"          ".size = %d,\n"
            "\t\t"          ".type = %d,\n"
            "\t\t"          ".stride = %d,\n"
            "\t\t"          ".divisor = %u\n"
            "\t"        "}"
            ,
            mesh->vbo_datas_arr[i].data_arr_size,
            name,
            i,
            mesh->vbo_datas_arr[i].size,
            mesh->vbo_datas_arr[i].type,
            mesh->vbo_datas_arr[i].stride,
            mesh->vbo_datas_arr[i].divisor
        );
    }
    fprintf(fp,
        ""      "\n};\n"
    );

    // mesh_indices_array
    fprintf(fp,
        ""      "static uint32_t %s_mesh_indices_array[] = {"
        ,
        name
    );
    for (uint64_t i = 0; i < mesh->indices_count; i++) {
        if (i != 0) fprintf(fp,", ");
        if (i%20 == 0) fprintf(fp,"\n\t");
        fprintf(fp,
            "%u",
            ((uint32_t*)(mesh->indices_array))[i]
        );
    }
    fprintf(fp, "\n};\n");

    // mesh_jointX_name
    for (uint64_t i = 0; i < mesh->joints_amount; i++) {
        fprintf(fp,
            ""      "static char %s_mesh_joint%llu_name[] = \"%s\";\n"
            ,
            name,
            i,
            mesh->joints[i].name
        );
    }

    // mesh_joints
    fprintf(fp,
        ""      "static joint_t %s_mesh_joints[] = {\n"
        ,
        name
    );
    for (uint64_t i = 0; i < mesh->joints_amount; i++) {
        if (i != 0) fprintf(fp, ",\n");
        fprintf(fp,
            "\t"        "{\n"
            "\t\t"          ".index = %u,\n"
            "\t\t"          ".name = %s_mesh_joint%llu_name,\n"
            "\t\t"          ".parent = %u,\n"
            "\t\t"          ".inverse_bind_transform_mat = (mat4_t){\n"
            "\t\t\t"            ".mat = {\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f\n"
            "\t\t\t"            "}\n"
            "\t\t"          "},\n"
            "\t\t"          ".local_transform_qvv = {\n"
            "\t\t\t"            ".rot = (quat_t){%f, %f, %f, %f},\n"
            "\t\t\t"            ".pos = (vec3_t){%f, %f, %f},\n"
            "\t\t\t"            ".scale = (vec3_t){%f, %f, %f}\n"
            "\t\t"          "},\n"
            "\t\t"          ".model_transform_mat = (mat4_t){\n"
            "\t\t\t"            ".mat = {\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f,\n"
            "\t\t\t\t"              "%f, %f, %f, %f\n"
            "\t\t\t"            "}\n"
            "\t\t"          "}\n"
            "\t"        "}"
            ,
            mesh->joints[i].index,
            name,
            i,
            mesh->joints[i].parent,
            mesh->joints[i].inverse_bind_transform_mat.mat[ 0],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 1],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 2],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 3],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 4],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 5],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 6],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 7],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 8],
            mesh->joints[i].inverse_bind_transform_mat.mat[ 9],
            mesh->joints[i].inverse_bind_transform_mat.mat[10],
            mesh->joints[i].inverse_bind_transform_mat.mat[11],
            mesh->joints[i].inverse_bind_transform_mat.mat[12],
            mesh->joints[i].inverse_bind_transform_mat.mat[13],
            mesh->joints[i].inverse_bind_transform_mat.mat[14],
            mesh->joints[i].inverse_bind_transform_mat.mat[15],
            mesh->joints[i].local_transform_qvv.rot.w,
            mesh->joints[i].local_transform_qvv.rot.x,
            mesh->joints[i].local_transform_qvv.rot.y,
            mesh->joints[i].local_transform_qvv.rot.z,
            mesh->joints[i].local_transform_qvv.pos.x,
            mesh->joints[i].local_transform_qvv.pos.y,
            mesh->joints[i].local_transform_qvv.pos.z,
            mesh->joints[i].local_transform_qvv.scale.x,
            mesh->joints[i].local_transform_qvv.scale.y,
            mesh->joints[i].local_transform_qvv.scale.z,
            mesh->joints[i].model_transform_mat.mat[ 0],
            mesh->joints[i].model_transform_mat.mat[ 1],
            mesh->joints[i].model_transform_mat.mat[ 2],
            mesh->joints[i].model_transform_mat.mat[ 3],
            mesh->joints[i].model_transform_mat.mat[ 4],
            mesh->joints[i].model_transform_mat.mat[ 5],
            mesh->joints[i].model_transform_mat.mat[ 6],
            mesh->joints[i].model_transform_mat.mat[ 7],
            mesh->joints[i].model_transform_mat.mat[ 8],
            mesh->joints[i].model_transform_mat.mat[ 9],
            mesh->joints[i].model_transform_mat.mat[10],
            mesh->joints[i].model_transform_mat.mat[11],
            mesh->joints[i].model_transform_mat.mat[12],
            mesh->joints[i].model_transform_mat.mat[13],
            mesh->joints[i].model_transform_mat.mat[14],
            mesh->joints[i].model_transform_mat.mat[15]
        );
    }
    fprintf(fp,
        ""      "\n};\n"
    );

    // mesh_pose_joint_transform_matrices
    fprintf(fp,
        ""      "static float %s_mesh_pose_joint_transform_matrices[] = {"
        ,
        name
    );
    for (uint64_t i = 0; i < mesh->joints_amount*16; i++) {
        if (i != 0) fprintf(fp,", ");
        if (i%16 == 0) fprintf(fp,"\n\t");
        fprintf(fp,
            "%f",
            mesh->pose_joint_transform_matrices[i]
        );
    }
    fprintf(fp,
        ""      "\n};\n"
    );

    // mesh 'name'
    fprintf(fp,
        ""      "mesh_t %s = (mesh_t){\n"
        "\t"        ".mesh_index = -1,\n"
        "\t"        ".saved = 1,\n"
        "\t"        ".unbinded = %hhu,\n"
        "\t"        ".vao = %u,\n"
        "\t"        ".vbos_amount = %u,\n"
        "\t"        ".vbos = %s_mesh_vbos,\n"
        "\t"        ".vbo_datas_arr = %s_mesh_vbo_datas_arr,\n"
        "\t"        ".indices_count = %u,\n"
        "\t"        ".indices_array = %s_mesh_indices_array,\n"
        "\t"        ".joints_amount = %u,\n"
        "\t"        ".joints = %s_mesh_joints,\n"
        "\t"        ".pose_joint_transform_matrices = %s_mesh_pose_joint_transform_matrices\n"
        ""      "};\n"
        ,
        name,
        mesh->unbinded,
        mesh->vao,
        mesh->vbos_amount,
        name,
        name,
        mesh->indices_count,
        name,
        mesh->joints_amount,
        name,
        name
    );

    fclose(fp);
}

void save_animation_to_c_file(animation_t* anim, const char* name, const char* file_path) {
    FILE* fp;
    fp = fopen(file_path, "w");
    if (fp == NULL) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open file \"%s\" to save animation \"%s\" to.\n", file_path, name);
        #endif
        return;
    }
    
    printf("saving animation \"%s\" to file \"%s\"\n", name, file_path);

    fprintf(fp, "#include <stdint.h>\n");

    // animation_jointX_key_frames
    for (uint64_t i = 0; i < anim->joints_amount; i++) {
        fprintf(fp,
            ""      "static key_frame_t %s_animation_joint%llu_key_frames[] = {"
            ,
            name,
            i
        );
        for (uint64_t j = 0; j < anim->joints_key_frames[i].key_frames_amount; j++) {
            if (j != 0) fprintf(fp, ",");
            fprintf(fp,
                "\n"
                "\t"        "{\n"
                "\t\t"          ".joint_local_transform = (mat4_t){\n"
                "\t\t\t"            ".mat = {\n"
                "\t\t\t\t"              "%f, %f, %f, %f,\n"
                "\t\t\t\t"              "%f, %f, %f, %f,\n"
                "\t\t\t\t"              "%f, %f, %f, %f,\n"
                "\t\t\t\t"              "%f, %f, %f, %f\n"
                "\t\t\t"            "}\n"
                "\t\t"          "},\n"
                "\t\t"          ".joint_local_transform_qvv = {\n"
                "\t\t\t"            ".rot = (quat_t){%f, %f, %f, %f},\n"
                "\t\t\t"            ".pos = (vec3_t){%f, %f, %f},\n"
                "\t\t\t"            ".scale = (vec3_t){%f, %f, %f}\n"
                "\t\t"          "},\n"
                "\t\t"          ".time_stamp = %f\n"
                "\t"        "}"
                ,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 0],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 1],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 2],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 3],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 4],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 5],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 6],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 7],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 8],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[ 9],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[10],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[11],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[12],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[13],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[14],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform.mat[15],
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.rot.w,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.rot.x,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.rot.y,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.rot.z,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.pos.x,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.pos.y,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.pos.z,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.scale.x,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.scale.y,
                anim->joints_key_frames[i].key_frames[j].joint_local_transform_qvv.scale.z,
                anim->joints_key_frames[i].key_frames[j].time_stamp
            );
        }
        fprintf(fp,
            "\n};\n"
        );
    }

    // animation_joints_key_frames
    fprintf(fp,
        ""      "static joint_key_frame_t %s_animation_joints_key_frames[] = {"
        ,
        name
    );
    for (uint64_t i = 0; i < anim->joints_amount; i++) {
        if (i != 0) fprintf(fp, ",");
        fprintf(fp,
            "\n"
            "\t"        "{\n"
            "\t\t"          ".key_frames_amount = %u,\n"
            "\t\t"          ".key_frames = %s_animation_joint%llu_key_frames\n"
            "\t"        "}"
            ,
            anim->joints_key_frames[i].key_frames_amount,
            name,
            i
        );
    }
    fprintf(fp,
        "\n};\n"
    );

    // animation 'name'
    fprintf(fp,
        ""      "animation_t %s = (animation_t){\n"
        "\t"        ".animation_index = -1,\n"
        "\t"        ".joints_amount = %u,\n"
        "\t"        ".joints_key_frames = %s_animation_joints_key_frames\n"
        ""      "};"
        ,
        name,
        anim->joints_amount,
        name
    );

    fclose(fp);
}