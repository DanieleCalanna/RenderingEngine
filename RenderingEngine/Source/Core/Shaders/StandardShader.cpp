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

StandardShader::StandardShader() :Shader("Resources/Shaders/StandardShader/StandardShaderVertex.glsl", "Resources/Shaders/StandardShader/StandardShaderFragment.glsl") { Refresh(); }

void StandardShader::Refresh()
{
	Shader::Refresh();

	Shader::Start();
	LoadInt("AlbedoMap", 0);
	LoadInt("SpecularMap", 1);
	LoadInt("RoughnessMap", 2);
	LoadInt("NormalMap", 3);
	LoadInt("AOMap", 4);
	LoadInt("EnvironmentMap", 5);
	LoadInt("IrradianceMap", 6);
	LoadInt("PrefilterMap", 7);
	LoadInt("BRDFLUT", 8);
	Shader::Stop();
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
			glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetPrefilteredCubemapTextureId());
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetIrradianceMapTextureId());
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetPrefilteredCubemapTextureId());
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, SkyBox->GetBRDFTextureId());
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
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, 0);
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