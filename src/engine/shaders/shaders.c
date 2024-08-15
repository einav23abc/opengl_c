#include "shaders.h"


#ifndef _SHADERS_MAX_AMOUNT
#define _SHADERS_MAX_AMOUNT_ (128)
#endif
uint64_t shaders_amount = 0;
int64_t current_shader = -1;
shader_t* shaders_list[_SHADERS_MAX_AMOUNT_];
const uint64_t SHADERS_MAX_AMOUNT = _SHADERS_MAX_AMOUNT_;


shader_t* create_shader(const char** vert_shader_str, const char** frag_shader_str,
                        const char* attribute_names , uint32_t attributes_count,
                        const char* uniform_names, uint32_t uniforms_count) {
    if (shaders_amount >= SHADERS_MAX_AMOUNT) return NULL;

    shader_t* shader = malloc(sizeof(shader_t));
    if (shader == NULL) return NULL;
    
    int32_t status;
    char err_buf[512];

    // <compile vertex shader>
        shader->vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader->vert_shader, 1, vert_shader_str, NULL);
        glCompileShader(shader->vert_shader);
        glGetShaderiv(shader->vert_shader, GL_COMPILE_STATUS, &status); // check glCompileShader sucess
        if (status != GL_TRUE) {
            glGetShaderInfoLog(shader->vert_shader, sizeof(err_buf), NULL, err_buf);
            err_buf[sizeof(err_buf)-1] = '\0';
            fprintf(stderr, "Vertex shader compilation failed: %s\nVertex shader:\n\"%s\"\n\n", err_buf, *vert_shader_str);
            free(shader);
            return NULL;
        }
    // </compile vertex shader>

    // <compile fragment shader>
        shader->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader->frag_shader, 1, frag_shader_str, NULL);
        glCompileShader(shader->frag_shader);
        glGetShaderiv(shader->frag_shader, GL_COMPILE_STATUS, &status); // check glCompileShader sucess
        if (status != GL_TRUE) {
            glGetShaderInfoLog(shader->frag_shader, sizeof(err_buf), NULL, err_buf);
            err_buf[sizeof(err_buf)-1] = '\0';
            fprintf(stderr, "Fragment shader compilation failed: %s\nFragment shader:\n\"%s\"\n\n", err_buf, *frag_shader_str);
            free(shader);
            return NULL;
        }
    // </compile fragment shader>
    
    // <attach shaders to the shader-program>
        shader->program = glCreateProgram();
        glAttachShader(shader->program, shader->vert_shader);
        glAttachShader(shader->program, shader->frag_shader);
    // </attach shaders to the shader-program>

    // <attributes location binding>
        uint32_t attribute_names_offset = 0;
        for (uint32_t i = 0; i < attributes_count; i++) {
            glBindAttribLocation(shader->program, i, &(attribute_names[attribute_names_offset]));
            attribute_names_offset += strlen(&(attribute_names[attribute_names_offset]))+1;
        }
    // </attributes location binding>

    // <link vertex and fragment shaders>
        glLinkProgram(shader->program);
        glGetProgramiv(shader->program, GL_LINK_STATUS, &status); // check glLinkProgram sucess
        if (status != GL_TRUE) {
            glGetProgramInfoLog(shader->program, sizeof(err_buf), NULL, err_buf);
            err_buf[sizeof(err_buf)-1] = '\0';
            fprintf(stderr, "Shader program linking failed: %s\nVertex shader:\n\"%s\"\n\nFragment shader:\n\"%s\"\n\n", err_buf, *vert_shader_str, *frag_shader_str);
            free(shader);
            return NULL;
        }
    // </link vertex and fragment shaders>

    // the shader was not updated by any camera yet
    shader->wvp_mat_camera_index = -1;
    
    // <shader default uniforms>
        shader->u_texture_loc                               = glGetUniformLocation(shader->program , "u_texture");
        shader->u_camera_world_view_projection_matrix_loc   = glGetUniformLocation(shader->program , "u_camera_world_view_projection_matrix");
        shader->u_instanced_drawing_float_data_loc          = glGetUniformLocation(shader->program , "u_instanced_drawing_float_data");
        shader->u_instanced_drawing_uint_data_loc           = glGetUniformLocation(shader->program , "u_instanced_drawing_uint_data");
        shader->u_joint_matrices_loc                        = glGetUniformLocation(shader->program , "u_joint_matrices");
    // </shader default uniforms>

    // <shader user defined uniforms>
        shader->uniform_locations = malloc(sizeof(int32_t)*uniforms_count);
        uint32_t uniform_names_offset = 0;
        for (uint32_t u = 0; u < uniforms_count; u++) {
            shader->uniform_locations[u] = glGetUniformLocation(shader->program , &(uniform_names[uniform_names_offset]));
            uniform_names_offset += strlen(&(uniform_names[uniform_names_offset]))+1;
        }
    // </shader user defined uniforms>

    // <append shader to shaders_list>
        *((uint64_t*)(&shader->shader_index)) = shaders_amount;
        shaders_list[shaders_amount] = shader;
        shaders_amount += 1;
    // </append shader to shaders_list>
    
    shader->active = 1; // set shader as active

    return shader;
}
shader_t* create_shader_from_files( const char* vert_shader_file_path, const char* frag_shader_file_path,
                                    const char* attribute_names , uint32_t attributes_count,
                                    const char* uniform_names, uint32_t uniforms_count) {
    char* vert_shader_str;
    load_file_contents(&vert_shader_str,vert_shader_file_path);
    if (vert_shader_str == NULL) return NULL;
    
    char* frag_shader_str;
    load_file_contents(&frag_shader_str,frag_shader_file_path);
    if (frag_shader_str == NULL) {
        free(vert_shader_str);
        return NULL;
    }

    shader_t* shader = create_shader((const char**)&vert_shader_str, (const char**)&frag_shader_str, attribute_names, attributes_count, uniform_names, uniforms_count);
    
    free(vert_shader_str);
    free(frag_shader_str);

    return shader;
}
void use_shader(shader_t* shader) {
    if (shader->shader_index == current_shader) return;
    if (shader->shader_index < 0 || shader->shader_index >= shaders_amount) return;

    glUseProgram(shader->program);
    current_shader = shader->shader_index;
    
    if (shader->wvp_mat_camera_index == current_camera) return;
    update_shader_camera_uniforms();
    shader->wvp_mat_camera_index = current_camera;
}
void update_shader_camera_uniforms() {
    if (current_shader < 0 || current_shader >= shaders_amount) return;
    if (current_camera < 0 || current_camera >= cameras_amount) return;

    if (shaders_list[current_shader]->u_camera_wvp_mat_loc == -1) return;

    glUniformMatrix4fv(shaders_list[current_shader]->u_camera_wvp_mat_loc, 1, 0, cameras_list[current_camera]->wvp_mat.mat);
}
static void clean_shader(shader_t* shader) {
    glDetachShader(shader->program, shader->vert_shader);
    glDetachShader(shader->program, shader->frag_shader);
    glDeleteProgram(shader->program);
    glDeleteShader(shader->vert_shader);
    glDeleteShader(shader->frag_shader);
    free(shader->uniform_locations);
    return;
}
int32_t destroy_shader(shader_t* shader) {
    if (current_shader == shader->shader_index) return -1;
    
    shaders_amount -= 1;

    // move the last shader at shaders_list to shader's index
    shader_t* last_shader = shaders_list[shaders_amount];
    if (current_shader == last_shader->shader_index){
        current_shader = shader->shader_index;
    }
    *((uint64_t*)&last_shader->shader_index) = shader->shader_index;
    shaders_list[shader->shader_index] = last_shader;

    clean_shader(shader);
    return 0;
}
void clean_shaders() {
    printf("cleaning %u shaders\n", shaders_amount);
    for (uint64_t i = 0; i < shaders_amount; i++) clean_shader(shaders_list[i]);
    shaders_amount = 0;
}