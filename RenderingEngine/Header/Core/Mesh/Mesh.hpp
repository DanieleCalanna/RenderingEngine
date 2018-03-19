#pragma once

#include <string>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "GL/glew.h"

class MeshRenderer;

class Mesh
{
	friend class MeshRenderer;

public:

	Mesh() {}
	Mesh(std::string ObjFilePath);

	virtual void Render();

	virtual void Clear();

	GLuint StoreDataInAttributeList(unsigned int AttributeIndex, unsigned int CoordinateSize, GLvoid* Data, int NumberOfElements);

protected:
	
	GLuint VertexArrayID;

	std::vector < glm::vec4 > Vertices;
	std::vector < glm::vec2 > UVs;
	std::vector < glm::vec4 > Normals; 

	void LoadObj(std::string ObjFilePath);

	virtual void InitBuffers();

};