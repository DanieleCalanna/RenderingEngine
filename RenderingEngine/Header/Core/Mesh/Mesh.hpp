#pragma once

class Mesh;

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <GL/glew.h>

class Mesh{

protected:
	
	GLuint vaoID;

	
	std::vector < glm::vec2 > uvs;


public:
	std::vector < glm::vec4 > vertices;
	std::vector < glm::vec4 > normals;

	Mesh(){}
	Mesh(std::vector < glm::vec4 >vertices, std::vector < glm::vec4 >normals);

	static Mesh* FromOBJFile(std::string path);

	virtual void InitBuffers();
	
	virtual void Render();

	virtual void Clear();

	GLuint storeDataInAttributeList(int attributeNumber, int coordinateSize, GLvoid * data, int size);

};