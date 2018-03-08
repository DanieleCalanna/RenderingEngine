
#define _USE_MATH_DEFINES // for C++  
#include <cmath>

#include "Core/Transform.hpp"
#include "Core/Camera/CameraController.hpp"
#include "Core/Entities/Actor.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Core/Window.hpp"
#include "glm/vec3.hpp"


void CameraController::Start()
{
	xRotation = (float)(GetOwner()->GetWorldTransform().Rotation[0] * 180 / M_PI);
}

void CameraController::Update()
{

	double MovementSpeed = (Window::GetWindow().yWheelOffset + 1) / 20 + 1;
	if (MovementSpeed < 1) MovementSpeed = 1;

	xRotation += (float)(Window::GetWindow().getMouseY() / 8);
	if (xRotation > 90) xRotation = 90;
	if (xRotation < -90) xRotation = -90;
	zRotation += (float)(Window::GetWindow().getMouseX() / 8);

	float x = (float)(sin(zRotation * M_PI / 180) * 0.5 *MovementSpeed);
	float y = (float)(sin(xRotation * M_PI / 180) * 0.5 *MovementSpeed);
	float z = (float)(cos(zRotation * M_PI / 180) * 0.5 *MovementSpeed);

	Transform OwnerWorldTransform = GetOwner()->GetWorldTransform();
	OwnerWorldTransform.Rotation.x = xRotation;
	OwnerWorldTransform.Rotation.y = zRotation;

	if (Window::GetWindow().GetKeyDown(GLFW_KEY_W))
	{
		OwnerWorldTransform.Location = glm::vec3(
			OwnerWorldTransform.Location.x + x,
			OwnerWorldTransform.Location.y - y,
			OwnerWorldTransform.Location.z - z
		);
	}
	if (Window::GetWindow().GetKeyDown(GLFW_KEY_S))
	{
		OwnerWorldTransform.Location = glm::vec3(
			OwnerWorldTransform.Location.x - x,
			OwnerWorldTransform.Location.y + y,
			OwnerWorldTransform.Location.z + z
		);
	}
	if (Window::GetWindow().GetKeyDown(GLFW_KEY_D))
	{
		OwnerWorldTransform.Location = glm::vec3(
			OwnerWorldTransform.Location.x + z,
			OwnerWorldTransform.Location.y,
			OwnerWorldTransform.Location.z + x
		);
	}
	if (Window::GetWindow().GetKeyDown(GLFW_KEY_A))
	{
		OwnerWorldTransform.Location = glm::vec3(
			OwnerWorldTransform.Location.x - z,
			OwnerWorldTransform.Location.y,
			OwnerWorldTransform.Location.z - x
		);
	}
	GetOwner()->SetWorldTransform(OwnerWorldTransform);
}