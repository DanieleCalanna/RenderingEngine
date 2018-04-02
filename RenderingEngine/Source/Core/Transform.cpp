#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++  
#endif

#include <cmath>
#include "Core/Transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"

#include <iostream>
#include<glm/gtx/string_cast.hpp>

Transform::Transform() {}

Transform::Transform(glm::vec3 Location, glm::quat Rotation, glm::vec3 Scale)
{
	this->Location = Location;
	this->Rotation = Rotation;
	this->Scale = Scale;
}

const glm::mat4x4 Transform::GetTransformationMatrix() const
{
	glm::mat4x4 TransformationMatrix;
	TransformationMatrix = glm::translate(glm::mat4x4(1.0), Location);
	TransformationMatrix *= glm::toMat4(Rotation);
	TransformationMatrix = glm::scale(TransformationMatrix, Scale);
	return TransformationMatrix;
}

Transform Transform::Combine(Transform const &Transform1, Transform const &Transform2)
{
	return Transform(
		Transform1.Location + Transform2.Location,
		Transform1.Rotation * Transform2.Rotation,
		Transform1.Scale * Transform2.Scale
	);
}

Transform Transform::Negate(Transform const & TransformToNegate)
{
	glm::vec3 ResultLocation(
		-TransformToNegate.Location.x,
		-TransformToNegate.Location.y,
		-TransformToNegate.Location.z
	);
	glm::vec3 ResultScale(
		1 / TransformToNegate.Scale.x,
		1 / TransformToNegate.Scale.y,
		1 / TransformToNegate.Scale.z
	);
	return Transform(ResultLocation, -TransformToNegate.Rotation, ResultScale);
}

glm::vec3 Transform::GetForwardVector() const
{
	return glm::normalize(glm::toMat3(Rotation)*glm::vec3(0.0f, 0.0f, -1.0f));
}
glm::vec3 Transform::GetRightVector() const
{
	return glm::normalize(glm::toMat3(Rotation)*glm::vec3(1.0f, 0.0f, 0.0f));
}
glm::vec3 Transform::GetUpVector() const
{
	return glm::normalize(glm::toMat3(Rotation)*glm::vec3(0.0f, 1.0f, 0.0f));
}

std::string Transform::ToString() const
{
	std::string LocationString("\tLocation: " + glm::to_string(Location) + "\n");
	std::string RotationString("\tRotation: " + glm::to_string(glm::normalize(Rotation)) + "\n");
	std::string ScaleString("\tScale: " + glm::to_string(Scale) + "\n");
	return std::string("Transform:\n") + LocationString + RotationString + ScaleString;
}