#include "frame_buffer_objects.h"

// https://learnopengl.com/Advanced-OpenGL/Framebuffers


#ifndef _FBOS_MAX_AMOUNT_
#define _FBOS_MAX_AMOUNT_ (128)
#endif
int32_t fbos_amount = 0;
int32_t current_fbo = 0;
fbo_t* fbos_list[_FBOS_MAX_AMOUNT_];
const uint32_t FBOS_MAX_AMOUNT = _FBOS_MAX_AMOUNT_;


static void clean_fbo(fbo_t* fbo) {
    if (fbo->color_status == 1) {
        glDeleteTextures(1, &(fbo->color));
    }else if (fbo->color_status == 2) {
        glDeleteRenderbuffers(1, &(fbo->color));
    }

    if (fbo->depth_stencil_status >= 1 && fbo->depth_stencil_status <= 3) {
        glDeleteTextures(1, &(fbo->depth_stencil));
    }else if (fbo->depth_stencil_status >= 4 && fbo->depth_stencil_status <= 6) {
        glDeleteRenderbuffers(1, &(fbo->depth_stencil));
    }
    
    glDeleteFramebuffers(1, &(fbo->fbo_id));
    free(fbo);
}
fbo_t* create_fbo_ext_param(uint32_t width, uint32_t height,
                            uint8_t color_status,
                            GLint color_format,
                            void(*color_texture_param_func)(),
                            uint8_t depth_stencil_status,
                            void(*depth_stencil_texture_param_func)()) {
    
    if (fbos_amount >= FBOS_MAX_AMOUNT) return NULL;
    
    // save binded frame buffer here
    uint64_t last_fbo = current_fbo;

    fbo_t* fbo = malloc(sizeof(fbo_t));
    if (fbo == NULL) return NULL;

    // create fbo
    glGenFramebuffers(1, &(fbo->fbo_id));
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_id);

    // color
    if (color_status == 1) { // texture
        // create texture
        glGenTextures(1, &(fbo->color));
        glBindTexture(GL_TEXTURE_2D, fbo->color);
        glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, NULL);
        
        // default texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // user set texture parameters
        if (color_texture_param_func != NULL) color_texture_param_func();

        // attach to frame buffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->color, 0);  
    }else if (color_status == 2) { // render buffer object
        // create render buffer object
        glGenRenderbuffers(1, &(fbo->color));
        glBindRenderbuffer(GL_RENDERBUFFER, fbo->color);
        glRenderbufferStorage(GL_RENDERBUFFER, color_format, width, height);

        // attach to frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo->color);
    }else {
        fbo->color = -1;
    }
    fbo->color_status = color_status;
    fbo->color_format = color_format;

    // depth and stencil
    if (depth_stencil_status == 1) { // texture, both depth and stencil
        // create texture
        glGenTextures(1, &(fbo->depth_stencil));
        glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

        // default texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // user set texture parameters
        if (depth_stencil_texture_param_func != NULL) depth_stencil_texture_param_func();

        // attach to frame buffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
    }else if (depth_stencil_status == 2) { // texture, only depth
        // create texture
        glGenTextures(1, &(fbo->depth_stencil));
        glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

        // default texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // user set texture parameters
        if (depth_stencil_texture_param_func != NULL) depth_stencil_texture_param_func();

        // attach to frame buffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
    }else if (depth_stencil_status == 3) { // texture, only stencil
        // create texture
        glGenTextures(1, &(fbo->depth_stencil));
        glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_INT, NULL);
        
        // default texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // user set texture parameters
        if (depth_stencil_texture_param_func != NULL) depth_stencil_texture_param_func();

        // attach to frame buffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
    }else if (depth_stencil_status == 4) { // render buffer object, both depth and stenchil
        // create render buffer object
        glGenRenderbuffers(1, &(fbo->depth_stencil));
        glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth_stencil);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        // attach to frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->depth_stencil);
    }else if (depth_stencil_status == 5) { // render buffer object, only depth
        // create render buffer object
        glGenRenderbuffers(1, &(fbo->depth_stencil));
        glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth_stencil);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        // attach to frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depth_stencil);
    }else if (depth_stencil_status == 6) { // render buffer object, only stenchil
        // create render buffer object
        glGenRenderbuffers(1, &(fbo->depth_stencil));
        glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth_stencil);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);

        // attach to frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->depth_stencil);
    }else {
        fbo->depth_stencil = -1;
    }
    fbo->depth_stencil_status = depth_stencil_status;

    fbo->width = width;
    fbo->height = height;

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("frame buffer object creation failed\n");
        clean_fbo(fbo);
        return NULL;
    }

    // append fbo to fbos_list
    *((uint64_t*)(&fbo->fbo_index)) = fbos_amount;
    fbos_list[fbos_amount] = fbo;
    fbos_amount += 1;

    // rebind last frame buffer here
    use_fbo(fbos_list[last_fbo]);

    return fbo;
}
fbo_t* create_fbo(  uint32_t width, uint32_t height,
                    uint8_t color_status,
                    GLint color_format,
                    uint8_t depth_stencil_status) {
    
    return create_fbo_ext_param(
        width, height,
        color_status,
        color_format,
        NULL,
        depth_stencil_status,
        NULL
    );
}
void use_default_fbo() {
    current_fbo = -1;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (current_camera != -1) update_camera_fbo_viewport(cameras_list[current_camera]);
}
void use_fbo(fbo_t* fbo) {
    current_fbo = fbo->fbo_index;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_id);
    if (current_camera != -1) update_camera_fbo_viewport(cameras_list[current_camera]);
}
void bind_fbo_color_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num) {
    glUniform1i(uniform, texture_num);
    glActiveTexture(GL_TEXTURE0+texture_num);
    glBindTexture(GL_TEXTURE_2D, fbo->color);
}
void bind_fbo_depth_stencil_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num) {
    glUniform1i(uniform, texture_num);
    glActiveTexture(GL_TEXTURE0+texture_num);
    glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
}
int32_t destroy_fbo(fbo_t* fbo) {
    if (current_fbo == fbo->fbo_index) return -1;

    fbos_amount -= 1;

    // move the last fbo at fbos_list to fbo's index
    fbo_t* last_fbo = fbos_list[fbos_amount];
    *((uint64_t*)&last_fbo->fbo_index) = fbo->fbo_index;
    fbos_list[last_fbo->fbo_index] = last_fbo;

    clean_fbo(fbo);
}
void clean_fbos() {
    printf("cleaning %u fbos\n", fbos_amount);
    for (uint64_t i = 0; i < fbos_amount; i++) clean_fbo(fbos_list[i]);
    fbos_amount = 0;
}