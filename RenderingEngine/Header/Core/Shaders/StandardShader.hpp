#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <Core/Lights/DirectionalLight.hpp>

class StandardShader {
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	
	StandardShader();

	GLuint createShaderFromFile(std::string path, GLenum shaderType);
	GLuint createProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

	std::string loadFile(std::string path);

public:

	void start();
	void stop();

	void loadColor(const glm::vec3 &color);
	void loadTransformationMatrix(glm::mat4x4 transformationMatrix);
	void loadProjectionMatrix(glm::mat4x4 projectionMatrix);
	void loadViewMatrix(glm::mat4x4 viewMatrix);
	void loadDirectionalLight(DirectionalLight directionalLight);

	static StandardShader& getInstance(){
		static StandardShader standardShader;
		return standardShader;
	}
};
