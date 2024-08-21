#ifndef GAME_MESHES_H
#define GAME_MESHES_H

#include "../engine/engine.h"

extern mesh_t* rect_plane_mesh;
extern mesh_t* cube_mesh;
extern mesh_t* centered_cube_mesh;
extern mesh_t* hinge_mesh;
extern mesh_t* tile_effect_mesh;
extern mesh_t* field_tile_mesh;
extern mesh_t* field_wheat_tile_mesh;
extern mesh_t* forest_tile_mesh;
extern mesh_t* forest_tree_tile_mesh;
extern mesh_t* mine_tile_mesh;
extern mesh_t* house_tile_mesh;
extern mesh_t* barracks_tile_mesh;
extern mesh_t* scale_base_mesh;
extern mesh_t* scale_head_mesh;

void game_meshes_init();

#endif