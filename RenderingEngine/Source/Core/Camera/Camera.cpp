#include "Core/Camera/Camera.hpp"
#include "Core/Camera/CameraComponent.hpp"
#include "glm/mat4x4.hpp"

const Camera* Camera::ActiveCamera = nullptr;

Camera::Camera()
{
	OwnedCameraComponent = AddComponent<CameraComponent>();
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
	return glm::mat4x4();
}

const Camera* Camera::GetActiveCamera()
{
	return ActiveCamera;
}
