#include "Core/Camera/Camera.hpp"
#include "Core/Camera/CameraComponent.hpp"
#include "Core/Camera/CameraController.hpp"
#include "Core/Transform.hpp"
#include "glm/mat4x4.hpp"

const Camera* Camera::ActiveCamera = nullptr;

Camera::Camera(std::string Name) : Actor(Name)
{
	OwnedCameraComponent = AddComponent<CameraComponent>("CameraComponent");
	OwnedCameraController = AddComponent<CameraController>("CameraController");
}

void Camera::SetActive() const
{
	Camera::ActiveCamera = this;
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
	return OwnedCameraComponent->GetProjectionMatrix();
}

glm::mat4x4 Camera::GetViewMatrix() const
{
	return OwnedCameraComponent->GetViewMatrix();
}

const Camera* Camera::GetActiveCamera()
{
	return ActiveCamera;
}
