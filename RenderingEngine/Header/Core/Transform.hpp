#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <string>

class Transform
{
public:
	glm::vec3 Location = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	Transform();
	Transform(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale);

	const glm::mat4x4 GetTransformationMatrix() const;

	static Transform Combine(Transform const &Transform1, Transform const &Transform2);
	static Transform Negate(Transform const &TransformToNegate);

	std::string ToString() const;
};
