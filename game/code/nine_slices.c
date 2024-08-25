#include "nine_slices.h"
#include "meshes.h"
#include "shaders.h"
#include "textures.h"

nine_slice_t nine_slice1;
nine_slice_t nine_slice2;
nine_slice_t nine_slice3;
nine_slice_t nine_slice4;

void nine_slices_init() {
    nine_slice1 = (nine_slice_t){
        .texture_width = 16,
        .texture_height = 16,
        .padding = 3,
        .texture = nine_slice1_texture
    };
    
    nine_slice2 = (nine_slice_t){
        .texture_width = 16,
        .texture_height = 16,
        .padding = 3,
        .texture = nine_slice2_texture
    };
    
    nine_slice3 = (nine_slice_t){
        .texture_width = 16,
        .texture_height = 16,
        .padding = 3,
        .texture = nine_slice3_texture
    };
    
    nine_slice4 = (nine_slice_t){
        .texture_width = 16,
        .texture_height = 16,
        .padding = 3,
        .texture = nine_slice4_texture
    };
}

void draw_nine_slice(nine_slice_t nine_slice, int32_t x, int32_t y, int32_t width, int32_t height) {
    shader_t* last_shader = shaders_list[current_shader];
    use_shader(nine_slice_shader);

    bind_texture(
        nine_slice.texture,
        shaders_list[current_shader]->u_texture_loc,
        0
    );

    float padding_x_uv_size = ((float)nine_slice.padding)/nine_slice.texture_width;
    float padding_y_uv_size = ((float)nine_slice.padding)/nine_slice.texture_height;

    // <corners>
        // u_scale
        glUniform2f(
            shaders_list[current_shader]->uniform_locations[1],
            nine_slice.padding,
            nine_slice.padding
        );
        // <bottom left>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x,
                y
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                0,
                1 - padding_y_uv_size,
                padding_x_uv_size,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // </bottom left>
        
        // <bottom right>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x + width - nine_slice.padding,
                y
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                1 - padding_x_uv_size,
                1 - padding_y_uv_size,
                padding_x_uv_size,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // </bottom right>

        // <top left>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x,
                y + height - nine_slice.padding
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                0,
                0,
                padding_x_uv_size,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // </top left>
        
        // <top right>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x + width - nine_slice.padding,
                y + height - nine_slice.padding
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                1 - padding_x_uv_size,
                0,
                padding_x_uv_size,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // </top right>
    // </corners>

    // <edges>
        // <bottom>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x + nine_slice.padding,
                y
            );
            // u_scale
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[1],
                width - nine_slice.padding*2,
                nine_slice.padding
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                padding_x_uv_size,
                1 - padding_y_uv_size,
                1 - padding_x_uv_size*2,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // <bottom>
        
        // <top>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x + nine_slice.padding,
                y + height - nine_slice.padding
            );
            // u_scale
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[1],
                width - nine_slice.padding*2,
                nine_slice.padding
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                padding_x_uv_size,
                0,
                1 - padding_x_uv_size*2,
                padding_y_uv_size
            );
            draw_mesh(rect_plane_mesh);
        // <top>
        
        // <left>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x,
                y + nine_slice.padding
            );
            // u_scale
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[1],
                nine_slice.padding,
                height - nine_slice.padding*2
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                0,
                padding_y_uv_size,
                padding_x_uv_size,
                1 - padding_y_uv_size*2
            );
            draw_mesh(rect_plane_mesh);
        // <left>
        
        // <right>
            // u_position
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[0],
                x + width - nine_slice.padding,
                y + nine_slice.padding
            );
            // u_scale
            glUniform2f(
                shaders_list[current_shader]->uniform_locations[1],
                nine_slice.padding,
                height - nine_slice.padding*2
            );
            // u_texcoord_change
            glUniform4f(
                shaders_list[current_shader]->uniform_locations[2],
                1 - padding_x_uv_size,
                padding_y_uv_size,
                padding_x_uv_size,
                1 - padding_y_uv_size*2
            );
            draw_mesh(rect_plane_mesh);
        // <right>
    // </edges>

    // <middle>
        // u_position
        glUniform2f(
            shaders_list[current_shader]->uniform_locations[0],
            x + nine_slice.padding,
            y + nine_slice.padding
        );
        // u_scale
        glUniform2f(
            shaders_list[current_shader]->uniform_locations[1],
            width - nine_slice.padding*2,
            height - nine_slice.padding*2
        );
        // u_texcoord_change
        glUniform4f(
            shaders_list[current_shader]->uniform_locations[2],
            padding_x_uv_size,
            padding_y_uv_size,
            1 - padding_x_uv_size*2,
            1 - padding_y_uv_size*2
        );
        draw_mesh(rect_plane_mesh);
    // </middle>

    use_shader(last_shader);
}