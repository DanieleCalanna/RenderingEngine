
#define _USE_MATH_DEFINES // for C++  
#include <cmath> 

#include "Core/Camera/CameraComponent.hpp"

#include "Core/Entities/Actor.hpp"
#include "Core/Window.hpp"
#include "Core/Transform.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

void CameraComponent::Update() 
{
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void CameraComponent::UpdateViewMatrix()
{
	ViewMatrix = new glm::mat4x4(1.0f);
	Transform CameraWorldTransform = GetOwner()->GetWorldTransform();
	*ViewMatrix = glm::rotate(*ViewMatrix, (float)(CameraWorldTransform.Rotation.x * M_PI / 180), vec3(1, 0, 0));
	*ViewMatrix = glm::rotate(*ViewMatrix, (float)(CameraWorldTransform.Rotation.y * M_PI / 180), vec3(0, 1, 0));
	*ViewMatrix = glm::rotate(*ViewMatrix, (float)(CameraWorldTransform.Rotation.z * M_PI / 180), vec3(0, 0, 1));
	*ViewMatrix = glm::translate(*ViewMatrix, -CameraWorldTransform.Location);
}

void CameraComponent::UpdateProjectionMatrix()
{
	ProjectionMatrix = new glm::mat4x4(0.0f);
	float AspectRatio = (float)Window::GetWindow().Width() / (float)Window::GetWindow().Height();
	float YScale = (float)((1.0f / tan((FieldOfView / 2.0f) * M_PI / 180)));
	float XScale = YScale / AspectRatio;
	float FrustumLength = FarPlane - NearPlane;
	(*ProjectionMatrix)[0][0] = XScale;
	(*ProjectionMatrix)[1][1] = YScale;
	(*ProjectionMatrix)[2][2] = -((FarPlane + NearPlane) / FrustumLength);
	(*ProjectionMatrix)[2][3] = -1;
	(*ProjectionMatrix)[3][2] = -((2 * NearPlane * FarPlane) / FrustumLength);
	(*ProjectionMatrix)[3][3] = 0;
}

glm::mat4x4 CameraComponent::GetProjectionMatrix() const { return *ProjectionMatrix; }
glm::mat4x4 CameraComponent::GetViewMatrix() const { return *ViewMatrix; }

CameraComponent::CameraComponent()
{
	ProjectionMatrix = new glm::mat4x4();
	ViewMatrix = new glm::mat4x4();
}
