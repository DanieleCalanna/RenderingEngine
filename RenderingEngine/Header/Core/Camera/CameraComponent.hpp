#pragma once

#include "Core/Components/Component.hpp"
#include "glm/fwd.hpp"

class CameraComponent : public Component
{
public:
	CameraComponent() :Component("CameraComponent") {}
	const glm::mat4x4 GetProjectionMatrix() const;
	const glm::mat4x4 GetViewMatrix() const;

private:

	float FieldOfView = 50.0f;
	float NearPlane = 0.1f;
	float FarPlane = 3000.0f;
};
