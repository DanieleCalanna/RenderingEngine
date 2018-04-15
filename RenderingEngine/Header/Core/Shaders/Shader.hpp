#pragma once

#include "GL/glew.h"
#include <string>
#include <glm/mat4x4.hpp>

class Shader
{
private:
	std::string LoadFile(std::string path) const;

protected:
	GLuint ProgramID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;

	std::string VertexShaderSourcePath;
	std::string FragmentShaderSourcePath;

	GLuint CreateShaderFromFile(std::string FilePath, GLenum ShaderType);
	GLuint CreateProgram(GLuint VertexShaderID, GLuint FragmentShaderID);

	bool bIsValid = false;
	bool bIsActive = false;

public:

	Shader(std::string InVertexShaderSourcePath, std::string InFragmentShaderSourcePath);

	virtual void Refresh();

	virtual void Start();
	virtual void Stop();

	virtual bool IsValid() const;
	bool IsActive() const;

	void LoadInt(std::string Location, GLint Value);
	void LoadFloat(std::string Location, GLfloat Value);
	void LoadMat4(std::string Location, glm::mat4x4 Matrix);
};
