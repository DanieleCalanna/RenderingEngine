#include "Core/Shaders/StandardShader.hpp"
#include "Core/Material/Material.hpp"
#include "Core/Texture/Texture.hpp"
#include "Core/Transform.hpp"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SkyBox.hpp"

StandardShader::StandardShader()
{
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

	glUseProgram(ProgramID);
	glUniform1i(glGetUniformLocation(ProgramID, "AlbedoMap"), 0);
	glUniform1i(glGetUniformLocation(ProgramID, "SpecularMap"), 1);
	glUniform1i(glGetUniformLocation(ProgramID, "RoughnessMap"), 2);
	glUniform1i(glGetUniformLocation(ProgramID, "NormalMap"), 3);
	glUniform1i(glGetUniformLocation(ProgramID, "AOMap"), 4);
	glUniform1i(glGetUniformLocation(ProgramID, "EnvironmentMap"), 5);
	glUseProgram(0);
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
	//glBindAttribLocation(ProgramID, 0, "position");
	//glBindAttribLocation(ProgramID, 1, "normal");
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

void StandardShader::Start(Material* MaterialToRender)
{
	if (bIsActive)
	{
		Stop();
	}
	CurrentMaterial = MaterialToRender;
	if (CurrentMaterial)
	{
		glUseProgram(ProgramID);

		CurrentMaterial->Activate(GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4);

		Scene* CurrentScene = Scene::GetCurrentScene();
		if (CurrentScene)
		{
			SkyBoxComponent* SkyBox = CurrentScene->GetComponent<SkyBoxComponent>();
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetCubeMapTextureId());
		}
		bIsActive = true;
	}

}
void StandardShader::Stop()
{
	if (bIsActive && CurrentMaterial)
	{
		CurrentMaterial->Deactivate();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUseProgram(0);
		bIsActive = false;
	}
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