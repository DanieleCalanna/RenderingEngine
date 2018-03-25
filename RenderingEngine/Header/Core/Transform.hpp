#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

#include <string>

class Transform
{
public:
	glm::vec3 Location = glm::vec3(0.0f);
	glm::quat Rotation = glm::quat();
	glm::vec3 Scale = glm::vec3(1.0f);

	Transform();
	Transform(glm::vec3 Location, glm::quat Rotation, glm::vec3 Scale);

	const glm::mat4x4 GetTransformationMatrix() const;

	static Transform Combine(Transform const &Transform1, Transform const &Transform2);
	static Transform Negate(Transform const &TransformToNegate);

	glm::vec3 GetForwardVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;

	std::string ToString() const;
};
