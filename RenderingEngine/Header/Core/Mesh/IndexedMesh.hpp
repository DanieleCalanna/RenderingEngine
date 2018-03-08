#pragma once

class IndexedMesh;

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <Core/Mesh/Mesh.hpp>

class IndexedMesh : public Mesh{

public:

	unsigned int nels, nadjs;
	unsigned int minAdjNum, maxAdjNum;
	float meanAdjNum;
	float *result_vertex4Array;
	unsigned int* adjArray;
	size_t memsize, adjmemsize;

	GLuint vertexBuffer;
	GLuint normalsBuffer;

	std::vector < unsigned int > indices;

	GLuint elementbuffer;

	IndexedMesh():Mesh(){}
	
	static Mesh* FromOBJFile(std::string path);

	virtual void InitBuffers();
	
	virtual void Render();

	virtual void Clear();
	
	GLuint storeDataInAttributeListShared(int attributeNumber, int coordinateSize, GLvoid * data, int size);
};