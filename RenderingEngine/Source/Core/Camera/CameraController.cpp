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

#include <string>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

void CameraController::Start()
{
	XRotation = (float)(GetOwner()->GetWorldTransform().Rotation[0] * 180 / M_PI);
}

void CameraController::Update()
{
	float MovementSpeed = (float)(Window::GetSingletonWindow().GetYWheelOffset() + 1) / 20 + 1;
	if (MovementSpeed < 0.2f) MovementSpeed = 0.2f;

	Transform OwnerWorldTransform = GetOwner()->GetWorldTransform();
	OwnerWorldTransform.Rotation = glm::normalize(glm::rotate(OwnerWorldTransform.Rotation, (float)(Window::GetSingletonWindow().GetMouseX() *0.001f), glm::vec3(0.0f, 1.0f, 0.0f)));
	OwnerWorldTransform.Rotation = glm::normalize(glm::rotate(OwnerWorldTransform.Rotation, (float)(Window::GetSingletonWindow().GetMouseY() *0.001f), OwnerWorldTransform.GetRightVector()));

	std::cout << glm::to_string(OwnerWorldTransform.GetUpVector()) <<std::endl;

	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_W))
	{
		OwnerWorldTransform.Location += OwnerWorldTransform.GetForwardVector()*MovementSpeed;
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_S))
	{
		OwnerWorldTransform.Location -= OwnerWorldTransform.GetForwardVector()*MovementSpeed;
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_D))
	{
		OwnerWorldTransform.Location += OwnerWorldTransform.GetRightVector()*MovementSpeed;
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_A))
	{
		OwnerWorldTransform.Location -= OwnerWorldTransform.GetRightVector()*MovementSpeed;
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_Q))
	{
		OwnerWorldTransform.Location -= glm::vec3(0.0f, 1.0f, 0.0f)*MovementSpeed;
	}
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_E))
	{
		OwnerWorldTransform.Location += glm::vec3(0.0f, 1.0f, 0.0f)*MovementSpeed;
	}
	GetOwner()->SetWorldTransform(OwnerWorldTransform);
}