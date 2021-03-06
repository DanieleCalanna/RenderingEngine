#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#include "Core/Transform.hpp"
#include "Core/Camera/CameraController.hpp"
#include "Core/Entities/Actor.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Core/Window.hpp"
#include "glm/vec3.hpp"
#include "Core/Camera/Camera.hpp"

CameraController::CameraController():Component("CameraController") {}

void CameraController::Start()
{

}

void CameraController::Update()
{
	float MovementSpeed = (float)(Window::GetSingletonWindow().GetYWheelOffset()+ 10.0f);
	if (MovementSpeed < 1.0f) MovementSpeed = 1.0f;

	Transform OwnerWorldTransform = GetOwner()->GetWorldTransform();
	OwnerWorldTransform.Rotation = glm::normalize(glm::rotate(OwnerWorldTransform.Rotation, (float)(-Window::GetSingletonWindow().GetMouseX() *0.001f), glm::inverse(glm::toMat3(OwnerWorldTransform.Rotation))*glm::vec3(0.0f, 1.0f, 0.0f)));
	OwnerWorldTransform.Rotation = glm::normalize(glm::rotate(OwnerWorldTransform.Rotation, (float)(-Window::GetSingletonWindow().GetMouseY() *0.001f), glm::vec3(1.0f, 0.0f, 0.0f)));

	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_W))
	{
		OwnerWorldTransform.Location += OwnerWorldTransform.GetForwardVector()*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_S))
	{
		OwnerWorldTransform.Location -= OwnerWorldTransform.GetForwardVector()*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_D))
	{
		OwnerWorldTransform.Location += OwnerWorldTransform.GetRightVector()*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_A))
	{
		OwnerWorldTransform.Location -= OwnerWorldTransform.GetRightVector()*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_Q))
	{
		OwnerWorldTransform.Location -= glm::vec3(0.0f, 1.0f, 0.0f)*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_E))
	{
		OwnerWorldTransform.Location += glm::vec3(0.0f, 1.0f, 0.0f)*MovementSpeed * Window::GetSingletonWindow().GetDeltaTime();
	}
	GetOwner()->SetWorldTransform(OwnerWorldTransform);
}