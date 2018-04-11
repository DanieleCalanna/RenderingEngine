#include "Core/Shaders/SkyBoxShader.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

SkyBoxShader::SkyBoxShader() : Shader("Resources/Shaders/SkyBoxShader/VertexSkyBoxShader.vert", "Resources/Shaders/SkyBoxShader/FragmentSkyBoxShader.frag")
{
	Refresh();
	//Start();

	/*TO-DO */

	//Stop();
}

void SkyBoxShader::LoadTransformationMatrix(glm::mat4x4 TransformationMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "transformationMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &TransformationMatrix[0][0]);
}

void SkyBoxShader::LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "projectionMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void SkyBoxShader::LoadViewMatrix(glm::mat4x4 ViewMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "viewMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ViewMatrix[0][0]);
}

SkyBoxShader& SkyBoxShader::GetInstance()
{
	static SkyBoxShader SkyBoxShaderSingleton;
	return SkyBoxShaderSingleton;
}
