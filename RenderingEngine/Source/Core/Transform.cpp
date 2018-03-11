#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++  
#endif

#include <cmath>
#include "Core/Transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include<glm/gtx/string_cast.hpp>

Transform::Transform() {}

Transform::Transform(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale)
{
	this->Location = Location;
	this->Rotation = Rotation;
	this->Scale = Scale;
}

const glm::mat4x4 Transform::GetTransformationMatrix() const
{
	glm::mat4x4 TransformationMatrix;
	TransformationMatrix = glm::translate(glm::mat4x4(1.0), Location);
	TransformationMatrix = glm::rotate(TransformationMatrix, (float)(Rotation.x * M_PI / 180), glm::vec3(1, 0, 0));
	TransformationMatrix = glm::rotate(TransformationMatrix, (float)(Rotation.y * M_PI / 180), glm::vec3(0, 1, 0));
	TransformationMatrix = glm::rotate(TransformationMatrix, (float)(Rotation.z * M_PI / 180), glm::vec3(0, 0, 1));
	TransformationMatrix = glm::scale(TransformationMatrix, Scale);
	return TransformationMatrix;
}

Transform Transform::Combine(Transform const &Transform1, Transform const &Transform2)
{
	glm::vec3 ResultLocation(
		Transform1.Location.x + Transform2.Location.x,
		Transform1.Location.y + Transform2.Location.y,
		Transform1.Location.z + Transform2.Location.z
	);
	glm::vec3 ResultRotation(
		Transform1.Rotation.x + Transform2.Rotation.x,
		Transform1.Rotation.y + Transform2.Rotation.y,
		Transform1.Rotation.z + Transform2.Rotation.z
	);
	glm::vec3 ResultScale(
		Transform1.Scale.x * Transform2.Scale.x,
		Transform1.Scale.y * Transform2.Scale.y,
		Transform1.Scale.z * Transform2.Scale.z
	);
	return Transform(ResultLocation, ResultRotation, ResultScale);
}

Transform Transform::Negate(Transform const & TransformToNegate)
{
	glm::vec3 ResultLocation(
		-TransformToNegate.Location.x,
		-TransformToNegate.Location.y,
		-TransformToNegate.Location.z
	);
	glm::vec3 ResultRotation(
		-TransformToNegate.Rotation.x,
		-TransformToNegate.Rotation.y,
		-TransformToNegate.Rotation.z
	);
	glm::vec3 ResultScale(
		1 / TransformToNegate.Scale.x,
		1 / TransformToNegate.Scale.y,
		1 / TransformToNegate.Scale.z
	);
	return Transform(ResultLocation, ResultRotation, ResultScale);
}

std::string Transform::ToString() const
{
	std::string LocationString("\tLocation: " + glm::to_string(Location) + "\n");
	std::string RotationString("\tRotation: " + glm::to_string(Rotation) + "\n");
	std::string ScaleString("\tScale: " + glm::to_string(Scale) + "\n");
	return std::string("Transform:\n") + LocationString + RotationString + ScaleString;
}