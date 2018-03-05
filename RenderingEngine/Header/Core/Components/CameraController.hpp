#pragma once

#include <Core/Components/Component.hpp>

#include <glm/vec3.hpp>
#include <cmath>
#include <iostream>


class CameraController : public Component {
	float xRotation = 0;
	float zRotation = 0;
public:
	void Start();
	void Update();
};
