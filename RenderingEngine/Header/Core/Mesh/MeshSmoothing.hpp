#pragma once

#include <GL/glew.h>
#include <CL/ocl_boiler.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#include <windows.h>

#include <Core/Components/Component.hpp>

class IndexedMesh;

class MeshSmoothing : public Component 
{
private:
	
public:
	IndexedMesh* Mesh;

	int iterations;
	float lambda;
	float mi;

	cl_platform_id platform;
	cl_device_id deviceID;
	cl_context context;
	cl_command_queue queue;
	cl_program program;

	cl_mem cl_vertex_buffer;
	cl_mem cl_normals_buffer;
	cl_mem cl_adjArray;
	cl_mem cl_result;
	cl_mem cl_normals_per_face;
	cl_mem cl_faceIndexes;

	cl_mem cl_faceIndex_per_vertex;
	cl_mem cl_faces_info_array;

	cl_kernel smooth_k;
	cl_kernel normals_per_face_k;
	cl_kernel normals_per_vertex_k;

	cl_event lock, unlock;


	MeshSmoothing();

	void Start();
	
	void Update();

	void Clear(){}

	void ApplySmooth();
};