#include "Core/Shaders/Shader.hpp"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <streambuf>


Shader::Shader(std::string InVertexShaderSourcePath, std::string InFragmentShaderSourcePath)
{
	VertexShaderSourcePath = InVertexShaderSourcePath;
	FragmentShaderSourcePath = InFragmentShaderSourcePath;
}

void Shader::Refresh()
{
	if (bIsValid)
	{
		glDeleteProgram(ProgramID);
	}

	VertexShaderID = CreateShaderFromFile(VertexShaderSourcePath, GL_VERTEX_SHADER);
	FragmentShaderID = CreateShaderFromFile(FragmentShaderSourcePath, GL_FRAGMENT_SHADER);

	if (VertexShaderID == 0 || FragmentShaderID == 0)
	{
		bIsValid = false;
		return;
	}

	ProgramID = CreateProgram(VertexShaderID, FragmentShaderID);
	if (ProgramID == 0)
	{
		bIsValid = false;
		return;
	}

	glValidateProgram(ProgramID);
	bIsValid = true;
}

GLuint Shader::CreateShaderFromFile(std::string FilePath, GLenum ShaderType)
{
	std::string ShaderSource = LoadFile(FilePath);
	//std::cout<<"Shader loaded: "<<path<<std::endl<<shaderSource<<std::endl;
	const GLuint ShaderID = glCreateShader(ShaderType);
	const GLchar* Source = (const GLchar *)ShaderSource.c_str();
	glShaderSource(ShaderID, 1, &Source, 0);
	glCompileShader(ShaderID);

	GLint IsCompiled = GL_FALSE;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &IsCompiled);

	if (IsCompiled == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &MaxLength); //Get Log Length

		char* InfoLog = new char[MaxLength];
		glGetShaderInfoLog(ShaderID, MaxLength, &MaxLength, InfoLog);
		glDeleteShader(ShaderID);
		std::cout << "COMPILING SHADER ERROR: " << FilePath << std::endl << InfoLog << std::endl;
		std::string OutputError = "COMPILING SHADER ERROR: ";
		OutputError = OutputError + FilePath;
		OutputError = OutputError + "\n";
		OutputError = OutputError + InfoLog;
		OutputError = OutputError + "\n";
		OutputDebugString(OutputError.c_str());
		return 0;
	}
	return ShaderID;
}

GLuint Shader::CreateProgram(GLuint VertexShaderID, GLuint FragmentShaderID)
{
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	GLint IsLinked = 0;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == GL_FALSE)
	{
		GLint MaxLength = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);

		//The maxLength includes the NULL character
		GLchar* InfoLog = new GLchar[MaxLength];
		glGetProgramInfoLog(ProgramID, MaxLength, &MaxLength, InfoLog);

		//We don't need the program anymore.
		glDeleteProgram(ProgramID);
		//Don't leak shaders either.
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		std::cout << "LINKING PROGRAM ERROR: " << std::endl << InfoLog << std::endl;
		delete[] InfoLog;

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
		return 0;
	}
	//Always detach shaders after a successful link.
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	return ProgramID;
}

void Shader::Start()
{
	if (!bIsActive)
	{
		bIsActive = true;
		glUseProgram(ProgramID);
	}
}
void Shader::Stop()
{
	if (bIsActive)
	{
		bIsActive = false;
		glUseProgram(0);
	}
}

bool Shader::IsValid() const { return bIsValid; }

bool Shader::IsActive() const { return bIsActive; }

void Shader::LoadInt(std::string Location, GLint Value)
{
	glUniform1i(glGetUniformLocation(ProgramID, Location.c_str()), Value);
}

void Shader::LoadMat4(std::string Location, glm::mat4x4 Matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, Location.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}

std::string Shader::LoadFile(std::string path) const // TO-DO Move away from here
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return str;
}