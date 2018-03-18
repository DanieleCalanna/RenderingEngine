#include <Core/Shaders/StandardShader.hpp>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <streambuf>

StandardShader::StandardShader()
{
	VertexShaderID = CreateShaderFromFile("Resources/Shaders/StandardShader/VertexStandardShader.vert", GL_VERTEX_SHADER);
	FragmentShaderID = CreateShaderFromFile("Resources/Shaders/StandardShader/FragmentStandardShader.frag", GL_FRAGMENT_SHADER);
	ProgramID = CreateProgram(VertexShaderID, FragmentShaderID);
	glValidateProgram(ProgramID);
	
	Start();

	/*TO-DO */
	glm::vec3 tempColor(0.2578f, 0.5117f, 0.95312f);
	LoadColor(tempColor);
	//glUniform3fv(glGetUniformLocation(programID, "color"), 1, &tempColor[0]);
	glUniform1f(glGetUniformLocation(ProgramID, "shineDamper"), 1.0f);
	glUniform1f(glGetUniformLocation(ProgramID, "reflectivity"), 0.4f);
	/*TO-DO End*/

	Stop();
}

GLuint StandardShader::CreateShaderFromFile(std::string FilePath, GLenum ShaderType)
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
		exit(1); //TO-DO
	}
	return ShaderID;
}

GLuint StandardShader::CreateProgram(GLuint VertexShaderID, GLuint FragmentShaderID)
{
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glBindAttribLocation(ProgramID, 0, "position");
	glBindAttribLocation(ProgramID, 1, "normal");
	glLinkProgram(ProgramID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar* infoLog = new GLchar[maxLength];
		glGetProgramInfoLog(ProgramID, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(ProgramID);
		//Don't leak shaders either.
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		std::cout << "LINKING PROGRAM ERROR: " << std::endl << infoLog << std::endl;
		delete[] infoLog;
		exit(1);
	}
	//Always detach shaders after a successful link.
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	return ProgramID;
}

void StandardShader::Start()
{
	glUseProgram(ProgramID);
}
void StandardShader::Stop()
{
	glUseProgram(0);
}

void StandardShader::LoadColor(const glm::vec3 &Color)
{
	glUniform3fv(glGetUniformLocation(ProgramID, "color"), 1, &Color[0]);
}

void StandardShader::LoadTransformationMatrix(glm::mat4x4 TransformationMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "transformationMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &TransformationMatrix[0][0]);
}

void StandardShader::LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "projectionMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void StandardShader::LoadViewMatrix(glm::mat4x4 ViewMatrix)
{
	GLint location = glGetUniformLocation(ProgramID, "viewMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ViewMatrix[0][0]);
}

void StandardShader::LoadDirectionalLight(DirectionalLight& DirectionalLightToLoad)
{
	auto Color = DirectionalLightToLoad.GetColor();
	auto Direction = DirectionalLightToLoad.GetDirection();
	GLint location = glGetUniformLocation(ProgramID, "lightColor");
	glUniform3fv(location, 1, &Color[0]);
	location = glGetUniformLocation(ProgramID, "lightDirection");
	glUniform3fv(location, 1, &Direction[0]);
}

StandardShader& StandardShader::GetInstance()
{
	static StandardShader StandardShaderSingleton;
	return StandardShaderSingleton;
}

std::string StandardShader::LoadFile(std::string path) // TO-DO Move away from here
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return str;
}