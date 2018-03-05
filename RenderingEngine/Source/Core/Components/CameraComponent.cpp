#include <Core/Components/CameraComponent.hpp>

#include <math.h>
#include <Core/Window.hpp>
#include <Core/Transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.14159265

CameraComponent::CameraComponent(){}
CameraComponent::~CameraComponent(){}

void CameraComponent::UpdateViewMatrix(){
	viewMatrix = mat4x4(1.0f);
	Transform cameraWorldTransform = gameObject->GetWorldTransform();
	viewMatrix = rotate(viewMatrix, cameraWorldTransform.Rotation.x*3.1415927f/180, vec3(1, 0, 0));
	viewMatrix = rotate(viewMatrix, cameraWorldTransform.Rotation.y*3.1415927f/180, vec3(0, 1, 0));
	viewMatrix = rotate(viewMatrix, cameraWorldTransform.Rotation.z*3.1415927f/180, vec3(0, 0, 1));
	viewMatrix = translate(viewMatrix, -cameraWorldTransform.Location);
}

void CameraComponent::UpdateProjectionMatrix(){	
	projectionMatrix = mat4x4(0.0f);
	float aspectRatio = (float)Window::GetWindow().Width() / (float)Window::GetWindow().Height();
	float y_scale = (float) ((1.0f / tan ( (FOV / 2.0f) * PI / 180.0f )));
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;
	projectionMatrix[0][0] = x_scale;
	projectionMatrix[1][1] = y_scale;
	projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[2][3] = -1;
	projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[3][3] = 0;
}
