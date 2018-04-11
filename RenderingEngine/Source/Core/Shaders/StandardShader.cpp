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

StandardShader::StandardShader() :Shader("Resources/Shaders/StandardShader/VertexStandardShader.vert", "Resources/Shaders/StandardShader/FragmentStandardShader.frag") { Refresh(); }

void StandardShader::Refresh()
{
	Shader::Refresh();

	glUseProgram(ProgramID);
	glUniform1i(glGetUniformLocation(ProgramID, "AlbedoMap"), 0);
	glUniform1i(glGetUniformLocation(ProgramID, "SpecularMap"), 1);
	glUniform1i(glGetUniformLocation(ProgramID, "RoughnessMap"), 2);
	glUniform1i(glGetUniformLocation(ProgramID, "NormalMap"), 3);
	glUniform1i(glGetUniformLocation(ProgramID, "AOMap"), 4);
	glUniform1i(glGetUniformLocation(ProgramID, "EnvironmentMap"), 5);
	glUseProgram(0);
}

void StandardShader::Start()
{
	if (IsValid())
	{
		Shader::Start();

		CurrentMaterial->Activate(GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4);

		Scene* CurrentScene = Scene::GetCurrentScene();
		if (CurrentScene)
		{
			SkyBoxComponent* SkyBox = CurrentScene->GetComponent<SkyBoxComponent>();
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetCubeMapTextureId());
		}
	}

}
void StandardShader::Stop()
{
	if (bIsActive)
	{
		CurrentMaterial->Deactivate();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		Shader::Stop();
	}
}

bool StandardShader::IsValid() const
{
	return Shader::IsValid() && CurrentMaterial;
}

void StandardShader::SetMaterial(Material * InMaterial)
{
	if(InMaterial)
	{
		if (CurrentMaterial)
		{
			CurrentMaterial->Deactivate();
		}
		CurrentMaterial = InMaterial;
		if (bIsActive)
		{
			CurrentMaterial->Activate(GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4);
		}
	}
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