#pragma once

#include "Core/Components/Component.hpp"
#include "glm/fwd.hpp"

class CameraComponent : public Component
{
public:

	const glm::mat4x4 GetProjectionMatrix() const;
	const glm::mat4x4 GetViewMatrix() const;

private:

	float FieldOfView = 70.0f;
	float NearPlane = 0.1f;
	float FarPlane = 3000.0f;
};
