#include "Core/Shaders/SkyBoxShader.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

SkyBoxShader::SkyBoxShader() : Shader("Resources/Shaders/SkyBoxShader/SkyBoxVertex.glsl", "Resources/Shaders/SkyBoxShader/SkyBoxFragment.glsl") { Refresh(); }

void SkyBoxShader::Refresh()
{
	Shader::Refresh();

	Start();
	LoadInt("CubeMap", 0);
	Stop();
}

void SkyBoxShader::LoadTransformationMatrix(glm::mat4x4 TransformationMatrix)
{
	GLint Location = glGetUniformLocation(ProgramID, "TransformationMatrix");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &TransformationMatrix[0][0]);
}

void SkyBoxShader::LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix)
{
	GLint Location = glGetUniformLocation(ProgramID, "ProjectionMatrix");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void SkyBoxShader::LoadViewMatrix(glm::mat4x4 ViewMatrix)
{
	GLint Location = glGetUniformLocation(ProgramID, "ViewMatrix");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &ViewMatrix[0][0]);
}

SkyBoxShader& SkyBoxShader::GetInstance()
{
	static SkyBoxShader SkyBoxShaderSingleton;
	return SkyBoxShaderSingleton;
}
