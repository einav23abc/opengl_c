#include "billboard.h"

uvec2_t get_billboard_box_pos(billboard_t billboard) {
    float x = 0;
    float y = 0;
    if (billboard.box_pos_from_world_pos == 1) {
        vec2_t screen_cord = outport_space_position_from_world_space(vec3(billboard.box_world_pos_x, billboard.box_world_pos_y, billboard.box_world_pos_z));
        x = screen_cord.x;
        y = screen_cord.y;
    }

    x += billboard.x;
    y += billboard.y;

    y -= billboard.box_height;

    return (uvec2_t){
        .x = x,
        .y = y
    };
}
void draw_billboard_shaded(billboard_t billboard) {
    uvec2_t pos = get_billboard_box_pos(billboard);
    
    // u_position
    glUniform2f(
        shaders_list[current_shader]->uniform_locations[0],
        pos.x,
        pos.y
    );
    // u_scale
    glUniform2f(
        shaders_list[current_shader]->uniform_locations[1],
        billboard.box_width,
        billboard.box_height
    );
    draw_mesh(rect_plane_mesh);
}


