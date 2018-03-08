#pragma once

class DirectionalLight;

#include <Core/Components/Component.hpp>
#include <glm/vec3.hpp>

class DirectionalLight : public Component
{
public:
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 direction = glm::vec3(100,-300,1);
	bool shadow = true;
};