#pragma once

#include "Core/Entities/Actor.hpp"
#include "glm/fwd.hpp"

class CameraComponent;
class CameraController;

class Camera : public Actor
{
public:
	Camera(std::string Name);

	void SetActive() const;

	glm::mat4x4 GetProjectionMatrix() const;
	glm::mat4x4 GetViewMatrix() const;

	static const Camera* GetActiveCamera();

private:

	CameraComponent * OwnedCameraComponent = nullptr;
	CameraController * OwnedCameraController = nullptr;

	static const Camera* ActiveCamera;
};