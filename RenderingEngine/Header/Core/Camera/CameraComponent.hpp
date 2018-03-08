#pragma once

#include "Core/Components/Component.hpp"
#include "glm/fwd.hpp"

class CameraComponent : public Component
{

public:

	virtual void Update() override;

	glm::mat4x4 GetProjectionMatrix() const;
	glm::mat4x4 GetViewMatrix() const;

	CameraComponent();

private:

	float FieldOfView = 70;
	float NearPlane = 0.1f;
	float FarPlane = 3000;

	glm::mat4x4* ProjectionMatrix = nullptr;
	glm::mat4x4* ViewMatrix = nullptr;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

};
