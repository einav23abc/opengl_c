#include <stdint.h>
static uint8_t rect_plane_mesh_mesh_vbo_data_arr0[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 0, 0, 0, 0, 0, 0
};
static uint8_t rect_plane_mesh_mesh_vbo_data_arr1[] = {
	0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 
	0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63
};
static uint32_t rect_plane_mesh_mesh_vbos[] = {
	0, 0
};
static vbo_data_t rect_plane_mesh_mesh_vbo_datas_arr[] = {
	{
		.data_arr_size = 48,
		.data_arr = rect_plane_mesh_mesh_vbo_data_arr0,
		.size = 3,
		.type = 5126,
		.stride = 12,
		.divisor = 0
	},
	{
		.data_arr_size = 32,
		.data_arr = rect_plane_mesh_mesh_vbo_data_arr1,
		.size = 2,
		.type = 5126,
		.stride = 8,
		.divisor = 0
	}
};
static uint32_t rect_plane_mesh_mesh_indices_array[] = {
	1, 0, 2, 2, 0, 3
};
static joint_t rect_plane_mesh_mesh_joints[] = {

};
static float rect_plane_mesh_mesh_pose_joint_transform_matrices[] = {
};
static mesh_t rect_plane_mesh_mesh = (mesh_t){
	.mesh_index = -1,
	.saved = 1,
	.unbinded = 1,
	.vao = 1836216166,
	.vbos_amount = 2,
	.vbos = rect_plane_mesh_mesh_vbos,
	.vbo_datas_arr = rect_plane_mesh_mesh_vbo_datas_arr,
	.indices_count = 6,
	.indices_array = rect_plane_mesh_mesh_indices_array,
	.joints_amount = 0,
	.joints = rect_plane_mesh_mesh_joints,
	.pose_joint_transform_matrices = rect_plane_mesh_mesh_pose_joint_transform_matrices
};
mesh_t* rect_plane_mesh = &rect_plane_mesh_mesh;
