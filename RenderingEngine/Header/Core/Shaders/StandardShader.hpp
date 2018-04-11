#pragma once

#include "Core/Shaders/Shader.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "Core/Lights/DirectionalLight.hpp"

class Material;

class StandardShader : public Shader
{
private:

	StandardShader();

	Material* CurrentMaterial = nullptr;

public:

	virtual void Refresh() override;

	virtual void Start() override;
	virtual void Stop() override;

	virtual bool IsValid() const override;

	void SetMaterial(Material* InMaterial);

	void LoadBaseColor(const glm::vec3 &color);
	void LoadTransformationMatrix(glm::mat4x4 TransformationMatrix);
	void LoadProjectionMatrix(glm::mat4x4 ProjectionMatrix);
	void LoadViewMatrix(glm::mat4x4 ViewMatrix);
	void LoadDirectionalLight(DirectionalLight& DirectionalLightToLoad);

	static StandardShader& GetInstance();
};
