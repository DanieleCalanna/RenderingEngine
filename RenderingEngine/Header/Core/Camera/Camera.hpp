#pragma once

#include "Core/Entities/Actor.hpp"
#include "glm/fwd.hpp"

class CameraComponent;

class Camera : public Actor
{
public:
	Camera();

	void SetActive() const;

	glm::mat4x4 GetProjectionMatrix() const;
	glm::mat4x4 GetViewMatrix() const;

	static const Camera* GetActiveCamera();

private:

	CameraComponent * OwnedCameraComponent = nullptr;

	static const Camera* ActiveCamera;
};