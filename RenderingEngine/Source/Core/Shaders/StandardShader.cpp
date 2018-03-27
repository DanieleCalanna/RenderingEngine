#include "Core/Shaders/StandardShader.hpp"
#include "Core/Texture/Texture.h"
#include "Core/Transform.hpp"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SkyBox.hpp"

StandardShader::StandardShader()
{
	Albedo = new Texture("D:/Download/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");
	Specular = new Texture("D:/Download/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga");
	Roughness = new Texture("D:/Download/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga");
	Normal = new Texture("D:/Download/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga");

	Refresh();
}

void StandardShader::Refresh()
{
	VertexShaderID = CreateShaderFromFile("Resources/Shaders/StandardShader/VertexStandardShader.vert", GL_VERTEX_SHADER);
	FragmentShaderID = CreateShaderFromFile("Resources/Shaders/StandardShader/FragmentStandardShader.frag", GL_FRAGMENT_SHADER);
	
	if (VertexShaderID == 0 || FragmentShaderID == 0) 
	{
		bIsValid = false;
		return; 
	}
	
	ProgramID = CreateProgram(VertexShaderID, FragmentShaderID);
	glValidateProgram(ProgramID);

	Start();
	glUniform1i(glGetUniformLocation(ProgramID, "Albedo"), 0);
	glUniform1i(glGetUniformLocation(ProgramID, "Specular"), 1);
	glUniform1i(glGetUniformLocation(ProgramID, "Roughness"), 2);
	glUniform1i(glGetUniformLocation(ProgramID, "Normal"), 3);
	glUniform1i(glGetUniformLocation(ProgramID, "EnvironmentMap"), 4);
	/*TO-DO */
	LoadBaseColor(glm::vec3(0.2578f, 0.5117f, 0.95312f));
	glUniform1f(glGetUniformLocation(ProgramID, "ShineDamper"), 1.0f);
	glUniform1f(glGetUniformLocation(ProgramID, "Reflectivity"), 0.4f);
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
		return 0;
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
	Albedo->Activate(GL_TEXTURE0);
	Specular->Activate(GL_TEXTURE1);
	Roughness->Activate(GL_TEXTURE2);
	Normal->Activate(GL_TEXTURE3);

	Scene* CurrentScene = Scene::GetCurrentScene();
	if (CurrentScene)
	{
		SkyBoxComponent* SkyBox = CurrentScene->GetComponent<SkyBoxComponent>();
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetCubeMapTextureId());
	}
}
void StandardShader::Stop()
{
	Albedo->Deactivate();
	Specular->Deactivate();
	Roughness->Deactivate();
	Normal->Deactivate();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}

bool StandardShader::IsValid()
{
	return bIsValid;
}

void StandardShader::LoadBaseColor(const glm::vec3 &Color)
{
	glUniform3fv(glGetUniformLocation(ProgramID, "BaseColor"), 1, &Color[0]);
}

void StandardShader::LoadTransformationMatrix(glm::mat4x4 TransformationMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "TransformationMatrix"), 1, GL_FALSE, &TransformationMatrix[0][0]);
}

void StandardShader::LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void StandardShader::LoadViewMatrix(glm::mat4x4 ViewMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "ViewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
}

void StandardShader::LoadDirectionalLight(DirectionalLight& DirectionalLightToLoad)
{
	auto Color = DirectionalLightToLoad.GetColor();
	auto Direction = DirectionalLightToLoad.GetWorldTransform().GetForwardVector();
	glUniform3fv(glGetUniformLocation(ProgramID, "LightColor"), 1, &Color[0]);
	glUniform3fv(glGetUniformLocation(ProgramID, "LightDirection"), 1, &Direction[0]);
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