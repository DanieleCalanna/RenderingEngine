#pragma once

#include "Core/Shaders/Shader.hpp"
#include "glm/mat4x4.hpp"
#include <string>

class SkyBoxShader : public Shader
{
private:

	SkyBoxShader();

public:

	void LoadTransformationMatrix(glm::mat4x4 TransformationMatrix);
	void LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix);
	void LoadViewMatrix(glm::mat4x4 ViewMatrix);

	static SkyBoxShader& GetInstance();
};