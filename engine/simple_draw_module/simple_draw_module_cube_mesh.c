#include <stdint.h>
static uint8_t simple_draw_module_cube_mesh_mesh_vbo_data_arr0[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63
};
static uint32_t simple_draw_module_cube_mesh_mesh_vbos[] = {
	0
};
static vbo_data_t simple_draw_module_cube_mesh_mesh_vbo_datas_arr[] = {
	{
		.data_arr_size = 96,
		.data_arr = simple_draw_module_cube_mesh_mesh_vbo_data_arr0,
		.size = 3,
		.type = 5126,
		.stride = 12,
		.divisor = 0
	}
};
static uint32_t simple_draw_module_cube_mesh_mesh_indices_array[] = {
	2, 6, 7, 2, 3, 7, 0, 4, 5, 0, 1, 5, 0, 2, 6, 0, 4, 6, 1, 3, 
	7, 1, 5, 7, 0, 2, 3, 0, 1, 3, 4, 6, 7, 4, 5, 7
};
static joint_t simple_draw_module_cube_mesh_mesh_joints[] = {

};
static float simple_draw_module_cube_mesh_mesh_pose_joint_transform_matrices[] = {
};
static mesh_t simple_draw_module_cube_mesh_mesh = (mesh_t){
	.mesh_index = -1,
	.saved = 1,
	.unbinded = 1,
	.vao = 0,
	.vbos_amount = 1,
	.vbos = simple_draw_module_cube_mesh_mesh_vbos,
	.vbo_datas_arr = simple_draw_module_cube_mesh_mesh_vbo_datas_arr,
	.indices_count = 36,
	.indices_array = simple_draw_module_cube_mesh_mesh_indices_array,
	.joints_amount = 0,
	.joints = simple_draw_module_cube_mesh_mesh_joints,
	.pose_joint_transform_matrices = simple_draw_module_cube_mesh_mesh_pose_joint_transform_matrices
};
mesh_t* simple_draw_module_cube_mesh = &simple_draw_module_cube_mesh_mesh;