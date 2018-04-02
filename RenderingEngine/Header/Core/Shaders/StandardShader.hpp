#pragma once

#include "GL/glew.h"
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <Core/Lights/DirectionalLight.hpp>

class Texture;

class StandardShader 
{
private:
	GLuint ProgramID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	
	StandardShader();

	GLuint CreateShaderFromFile(std::string FilePath, GLenum ShaderType);
	GLuint CreateProgram(GLuint VertexShaderID, GLuint FragmentShaderID);

	std::string LoadFile(std::string path);

	Texture* Albedo = nullptr;
	Texture* Specular = nullptr;
	Texture* Roughness = nullptr;
	Texture* Normal = nullptr;
	Texture* AO = nullptr;

	bool bIsValid = false;

public:

	void Refresh();

	void Start();
	void Stop();

	bool IsValid();

	void LoadBaseColor(const glm::vec3 &color);
	void LoadTransformationMatrix(glm::mat4x4 TransformationMatrix);
	void LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix);
	void LoadViewMatrix(glm::mat4x4 ViewMatrix);
	void LoadDirectionalLight(DirectionalLight& DirectionalLightToLoad);

	static StandardShader& GetInstance();
};
