#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++  
#endif

#include <cmath>
#include "Core/Camera/CameraComponent.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Window.hpp"
#include "Core/Transform.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::mat4x4 CameraComponent::GetProjectionMatrix() const
{
	glm::mat4x4 ProjectionMatrix(0.0f);
	float AspectRatio = (float)Window::GetSingletonWindow().GetWidth() / (float)Window::GetSingletonWindow().GetHeight();
	float YScale = (float)((1.0f / tan((FieldOfView / 2.0f) * M_PI / 180.0f)));
	float XScale = YScale / AspectRatio;
	float FrustumLength = FarPlane - NearPlane;
	ProjectionMatrix[0][0] = XScale;
	ProjectionMatrix[1][1] = YScale;
	ProjectionMatrix[2][2] = -((FarPlane + NearPlane) / FrustumLength);
	ProjectionMatrix[2][3] = -1.0f;
	ProjectionMatrix[3][2] = -((2 * NearPlane * FarPlane) / FrustumLength);
	ProjectionMatrix[3][3] = 0.0f;
	return ProjectionMatrix;
}

const glm::mat4x4 CameraComponent::GetViewMatrix() const 
{
	glm::mat4x4 ViewMatrix(1.0f);
	Transform CameraWorldTransform = GetOwner()->GetWorldTransform();
	ViewMatrix = glm::rotate(ViewMatrix, (float)(CameraWorldTransform.Rotation.x * M_PI / 180), glm::vec3(1, 0, 0));
	ViewMatrix = glm::rotate(ViewMatrix, (float)(CameraWorldTransform.Rotation.y * M_PI / 180), glm::vec3(0, 1, 0));
	ViewMatrix = glm::rotate(ViewMatrix, (float)(CameraWorldTransform.Rotation.z * M_PI / 180), glm::vec3(0, 0, 1));
	ViewMatrix = glm::translate(ViewMatrix, -CameraWorldTransform.Location);
	return ViewMatrix;
}