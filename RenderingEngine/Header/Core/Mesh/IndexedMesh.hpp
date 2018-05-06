#pragma once

class IndexedMesh;

#include <string>
#include <vector>
#include <GL/glew.h>
#include <Core/Mesh/Mesh.hpp>

class IndexedMesh : public Mesh{

public:

	IndexedMesh() {}
	IndexedMesh(std::string ObjFilePath);

	unsigned int nels, nadjs;
	unsigned int minAdjNum, maxAdjNum;
	float meanAdjNum;
	float *result_vertex4Array;
	unsigned int* adjArray;
	size_t memsize, adjmemsize;

	GLuint vertexBuffer;
	GLuint normalsBuffer;

	std::vector < unsigned int > Indices;

	GLuint elementbuffer;
	

protected:

	void LoadObj(std::string ObjFilePath);

	virtual void InitBuffers() override;
	
	virtual void Render() override;

	virtual void Clear() override;
	
	GLuint StoreDataInAttributeListShared(unsigned int AttributeIndex, int CoordinateSize, GLvoid * Data, int NumberOfElements);

};