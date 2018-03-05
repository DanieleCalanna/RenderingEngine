#pragma once

#include <Core/Components/Component.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

using namespace glm;

class CameraComponent : public Component {
private:
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
public:
	
	float FOV = 70;
	float NEAR_PLANE = 0.1f;
	float FAR_PLANE = 3000;
	
	CameraComponent();
	~CameraComponent();

	void Start(){}
	
	void Update(){
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	
	mat4x4 GetProjectionMatrix() { return projectionMatrix; }
	mat4x4 GetViewMatrix() { return viewMatrix; }

	
	static CameraComponent& getInstance(){
		static CameraComponent currentCamera;
		return currentCamera;
	}
};
