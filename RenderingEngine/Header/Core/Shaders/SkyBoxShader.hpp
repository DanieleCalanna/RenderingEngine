#pragma once

#include "GL/glew.h"
#include <glm/mat4x4.hpp>
#include <string>

class SkyBoxShader
{
private:
	GLuint ProgramID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;

	SkyBoxShader();

	GLuint CreateShaderFromFile(std::string FilePath, GLenum ShaderType);
	GLuint CreateProgram(GLuint VertexShaderID, GLuint FragmentShaderID);

	std::string LoadFile(std::string path);

public:

	void Start();
	void Stop();

	void LoadTransformationMatrix(glm::mat4x4 TransformationMatrix);
	void LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix);
	void LoadViewMatrix(glm::mat4x4 ViewMatrix);



	static SkyBoxShader& GetInstance();
};