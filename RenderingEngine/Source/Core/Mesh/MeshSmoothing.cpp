#include "Core/Mesh/MeshSmoothing.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Mesh/IndexedMesh.hpp"
#include "Core/Mesh/MeshRenderer.hpp"
#include <cstdio>
#include <iostream>
#include <string>

#define OCL_FILENAME "Resources/Kernels/meshsmooth.ocl"


size_t preferred_wg_smooth;
size_t preferred_wg_normals_per_face;
size_t preferred_wg_normals_per_vertex;

void orderedUniqueInsert(std::vector< unsigned int >*adjacents, unsigned int vertexID) {
	std::vector< unsigned int >::iterator it;
	for(it = adjacents->begin(); it != adjacents->end() && *it < vertexID; it++) {}
	if(it == adjacents->end() || *it != vertexID){
		adjacents->insert(it, vertexID);
	}
}

cl_event smooth(cl_command_queue queue, cl_kernel smooth_k, cl_event* waitList, cl_mem cl_vertex4Array, cl_mem cl_adjArray, cl_mem cl_result, cl_int nels, cl_float factor){
	size_t gws[] = { round_mul_up(nels, preferred_wg_smooth) };
	cl_event smooth_evt;
	cl_int err;

	// Setting arguments
	err = clSetKernelArg(smooth_k, 0, sizeof(cl_vertex4Array), &cl_vertex4Array);
	ocl_check(err, "set smooth arg 0");
	err = clSetKernelArg(smooth_k, 1, sizeof(cl_adjArray), &cl_adjArray);
	ocl_check(err, "set smooth arg 1");
	err = clSetKernelArg(smooth_k, 2, sizeof(cl_result), &cl_result);
	ocl_check(err, "set smooth arg 2");
	err = clSetKernelArg(smooth_k, 3, sizeof(nels), &nels);
	ocl_check(err, "set smooth arg 3");
	err = clSetKernelArg(smooth_k, 4, sizeof(factor), &factor);
	ocl_check(err, "set smooth arg 4");

	err = clEnqueueNDRangeKernel(queue, smooth_k,
		1, NULL, gws, NULL, /* griglia di lancio */
		waitList==NULL?0:1, waitList, /* waiting list */
		&smooth_evt);
	ocl_check(err, "enqueue kernel smooth");
	return smooth_evt;
}

cl_event normals_per_face(
	cl_command_queue queue,
	cl_kernel normals_per_face_k,
	cl_event* waitList, cl_mem cl_vertex4Array,
	cl_mem faceIndexes,
	cl_mem cl_result,
	cl_int nels)
{
	size_t gws[] = { round_mul_up(nels, preferred_wg_normals_per_face) };
	cl_event normals_per_face_evt;
	cl_int err;

	// Setting arguments
	err = clSetKernelArg(normals_per_face_k, 0, sizeof(cl_vertex4Array), &cl_vertex4Array);
	ocl_check(err, "set smooth arg 0");
	err = clSetKernelArg(normals_per_face_k, 1, sizeof(faceIndexes), &faceIndexes);
	ocl_check(err, "set smooth arg 1");
	err = clSetKernelArg(normals_per_face_k, 2, sizeof(cl_result), &cl_result);
	ocl_check(err, "set smooth arg 2");
	err = clSetKernelArg(normals_per_face_k, 3, sizeof(nels), &nels);
	ocl_check(err, "set smooth arg 3");

	err = clEnqueueNDRangeKernel(queue, normals_per_face_k,
		1, NULL, gws, NULL, /* griglia di lancio */
		waitList==NULL?0:1, waitList, /* waiting list */
		&normals_per_face_evt);
	ocl_check(err, "enqueue kernel normals_per_face");
	return normals_per_face_evt;
}

cl_event normals_per_vertex(
	cl_command_queue queue,
	cl_kernel normals_per_vertex_k,
	cl_event* waitList, 
	cl_mem normals_per_face,
	cl_mem facesInfo_array,
	cl_mem faces_array,
	cl_mem gl_normals,
	cl_int nels)
{
	size_t gws[] = { round_mul_up(nels, preferred_wg_normals_per_vertex) };
	cl_event normals_per_vertex_evt;
	cl_int err;

	// Setting arguments
	err = clSetKernelArg(normals_per_vertex_k, 0, sizeof(normals_per_face), &normals_per_face);
	ocl_check(err, "set smooth arg 0");
	err = clSetKernelArg(normals_per_vertex_k, 1, sizeof(facesInfo_array), &facesInfo_array);
	ocl_check(err, "set smooth arg 1");
	err = clSetKernelArg(normals_per_vertex_k, 2, sizeof(faces_array), &faces_array);
	ocl_check(err, "set smooth arg 2");
	err = clSetKernelArg(normals_per_vertex_k, 3, sizeof(gl_normals), &gl_normals);
	ocl_check(err, "set smooth arg 3");
	err = clSetKernelArg(normals_per_vertex_k, 4, sizeof(nels), &nels);
	ocl_check(err, "set smooth arg 4");

	err = clEnqueueNDRangeKernel(queue, normals_per_vertex_k,
		1, NULL, gws, NULL, /* griglia di lancio */
		waitList==NULL?0:1, waitList, /* waiting list */
		&normals_per_vertex_evt);
	ocl_check(err, "enqueue kernel normals_per_vertex");
	return normals_per_vertex_evt;
}

MeshSmoothing::MeshSmoothing(IndexedMesh* Mesh) : Component("MeshSmoothingComponent")
{
	iterationsCounter = 0;
	iterationsPerFrame = 1;
	lambda = 0.5f;
	mi = -0.500001f;
	this->Mesh = Mesh;
}


#define CL_USE_DEPRECATED_OPENCL_1_1_APIS

void MeshSmoothing::Start()
{
	cl_int err;
	printf("\n======= BOILER INFO ========\n");
	platform = select_platform(1);

	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &deviceID, NULL );

	cl_context_properties props[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
		0
	};
	
	context = clCreateContext(props, 1, &deviceID, NULL, NULL, NULL);

	queue = create_queue(context, deviceID);
	program = create_program(OCL_FILENAME, context, deviceID);
	printf("============================\n");

	//MeshRenderer* OwnerMeshRenderer = (MeshRenderer*) GetOwner()->GetComponent<MeshRenderer>();
	//Mesh = (IndexedMesh*)OwnerMeshRenderer->GetMesh();


	Mesh->meanAdjNum = Mesh->nadjs/(float)Mesh->nels;
	Mesh->memsize = 4*Mesh->nels*sizeof(float);
	Mesh->adjmemsize = Mesh->nadjs*sizeof(unsigned int);
		
	printf("====== SMOOTHING INFO ======\n");
	std::cout << " # Iterations: " << iterationsPerFrame << std::endl;
	std::cout << " Lambda factor: " << lambda << std::endl;
	std::cout << " Mi factor: " << mi << std::endl;
	printf("============================\n");
		
	printf("========= OBJ INFO =========\n");
	std::cout << " # Vertex: " << Mesh->nels << std::endl;
	std::cout << " # Adjacents: " << Mesh->nadjs << std::endl;
	std::cout << " # Min vertex adjs: " << Mesh->minAdjNum << std::endl;
	std::cout << " # Max vertex adjs: " << Mesh->maxAdjNum << std::endl;
	std::cout << " # Mean vertex adjs: " << Mesh->meanAdjNum << std::endl;
	printf("============================\n\n");
			
	// Create Buffers
	cl_vertex_buffer = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, Mesh->vertexBuffer, &err);
	cl_normals_buffer = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, Mesh->normalsBuffer, &err);
	cl_adjArray = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, Mesh->adjmemsize, Mesh->adjArray, &err);
	cl_result = clCreateBuffer(context, CL_MEM_READ_WRITE, Mesh->memsize, NULL, &err);
	cl_normals_per_face = clCreateBuffer(context, CL_MEM_READ_WRITE, Mesh->Indices.size()/3*sizeof(float)*4, NULL, &err);

	std::vector < unsigned int > *faces_of_each_vertex = new std::vector < unsigned int >[Mesh->nels];
	int faceIndexesDim = ((int)Mesh->Indices.size())/3*4;
	unsigned int* faceIndexes = new unsigned int[faceIndexesDim];
	for(int i=0; i<faceIndexesDim/4; i++){
		unsigned int vertex1ID = Mesh->Indices[i*3];
		unsigned int vertex2ID = Mesh->Indices[i*3+1];
		unsigned int vertex3ID = Mesh->Indices[i*3+2];
		faces_of_each_vertex[vertex1ID].push_back(i);
		faces_of_each_vertex[vertex2ID].push_back(i);
		faces_of_each_vertex[vertex3ID].push_back(i);
		faceIndexes[i*4] = vertex1ID;
		faceIndexes[i*4+1] = vertex2ID;
		faceIndexes[i*4+2] = vertex3ID;
		faceIndexes[i*4+3] = 0;
		//printf("face %d -> %d / %d / %d\n", i, faceIndexes[i*4], faceIndexes[i*4+1], faceIndexes[i*4+2]);
	}

	int *faces_info_array = new int[Mesh->nels*2];
	unsigned int currentfaceIndex = 0;
	for(unsigned int i=0; i<Mesh->nels; i++){

		std::vector < unsigned int > faces_of_the_vertex = faces_of_each_vertex[i];

		faces_info_array[i*2] = currentfaceIndex;
		faces_info_array[i*2+1] = (unsigned int)faces_of_the_vertex.size();
		//printf("vertex %d currentFaceIndex %d size %d\n", i, currentfaceIndex, faces_of_the_vertex.size());
		currentfaceIndex += (unsigned int)faces_of_the_vertex.size();

	}
	unsigned int *faceIndex_per_vertex = new unsigned int[currentfaceIndex];
	int index_per_vertex = 0;
	for(unsigned int i=0; i<Mesh->nels; i++)
	{
		std::vector < unsigned int > faces_of_the_vertex = faces_of_each_vertex[i];
		for(unsigned int x : faces_of_the_vertex)
			faceIndex_per_vertex[index_per_vertex++] = x;
	}

	cl_faces_info_array = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, Mesh->nels*2*sizeof(int), faces_info_array, &err);
	cl_faceIndex_per_vertex = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, currentfaceIndex*sizeof(unsigned int), faceIndex_per_vertex, &err);
	cl_faceIndexes = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, faceIndexesDim*sizeof(int), faceIndexes, &err);

	// Extract kernels
	smooth_k = clCreateKernel(program, "smooth", &err);
	ocl_check(err, "create kernel smooth");
	normals_per_face_k = clCreateKernel(program, "normals_per_face_k", &err);
	ocl_check(err, "create kernel normals_per_face_k");
	normals_per_vertex_k = clCreateKernel(program, "normals_per_vertex_k", &err);
	ocl_check(err, "create kernel normals_per_vertex_k");
	

	// Set preferred_wg size from device info
	err = clGetKernelWorkGroupInfo(smooth_k, deviceID, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(preferred_wg_smooth), &preferred_wg_smooth, NULL);
	ocl_check(err, "clGetKernelWorkGroupInfo smooth");
	err = clGetKernelWorkGroupInfo(normals_per_face_k, deviceID, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(preferred_wg_normals_per_face), &preferred_wg_normals_per_face, NULL);
	ocl_check(err, "clGetKernelWorkGroupInfo normals_per_face");
	err = clGetKernelWorkGroupInfo(normals_per_vertex_k, deviceID, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(preferred_wg_normals_per_vertex), &preferred_wg_normals_per_vertex, NULL);
	ocl_check(err, "clGetKernelWorkGroupInfo normals_per_vertex");

}

void MeshSmoothing::ApplySmooth(){
	glFinish();
	cl_mem buffersToAquire[] = {cl_vertex_buffer, cl_normals_buffer};
	clEnqueueAcquireGLObjects(queue, 2, &cl_vertex_buffer, 0, NULL, &lock);

	int cl_event_count = 2 * iterationsPerFrame;
	cl_event * cl_event_array = new cl_event[cl_event_count];

	for(int i=0; i<iterationsPerFrame; i++) {
		cl_event smooth_evt = smooth(queue, smooth_k, &lock, cl_vertex_buffer, cl_adjArray, cl_result, Mesh->nels, lambda);
		lock = smooth(queue, smooth_k, &smooth_evt, cl_result, cl_adjArray, cl_vertex_buffer, Mesh->nels, mi);
		cl_event_array[i*2] = smooth_evt;
		cl_event_array[i*2+1] = lock;
	}
	iterationsCounter += iterationsPerFrame;
	cl_event normals_per_face_evt = normals_per_face(queue, normals_per_face_k, &lock, cl_vertex_buffer, cl_faceIndexes, cl_normals_per_face, ((cl_int)Mesh->Indices.size())/3);
	cl_event normals_per_vertex_evt = normals_per_vertex(queue, normals_per_vertex_k, &normals_per_face_evt, cl_normals_per_face, cl_faces_info_array, cl_faceIndex_per_vertex, cl_normals_buffer, Mesh->nels);
	clEnqueueReleaseGLObjects(queue, 2, buffersToAquire, 1, &normals_per_vertex_evt, &unlock);
	clWaitForEvents(1, &unlock);

	clReleaseEvent(normals_per_face_evt);
	clReleaseEvent(normals_per_vertex_evt);
	clReleaseEvent(unlock);
	for (int i = 0; i < cl_event_count; i++)
	{
		clReleaseEvent(cl_event_array[i]);
	}
	delete[] cl_event_array;
}


void MeshSmoothing::Update()
{
	ApplySmooth();
}