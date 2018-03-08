#include "Core/Transform.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#include <iostream>
#include<glm/gtx/string_cast.hpp>

using namespace glm;

Transform::Transform(){}
Transform::~Transform(){}

Transform::Transform(vec3 Location, vec3 Rotation, vec3 Scale)
{
	this->Location = Location;
	this->Rotation = Rotation;
	this->Scale = Scale;
}


mat4x4 Transform::GetTransformationMatrix()
{
	transformationMatrix = translate(mat4(1.0), Location);
	transformationMatrix = rotate(transformationMatrix, Rotation.x*3.1415927f/180.0f, vec3(1, 0, 0));
	transformationMatrix = rotate(transformationMatrix, Rotation.y*3.1415927f/180.0f, vec3(0, 1, 0));
	transformationMatrix = rotate(transformationMatrix, Rotation.z*3.1415927f/180.0f, vec3(0, 0, 1));
	transformationMatrix = scale(transformationMatrix, Scale);
	return transformationMatrix;
}


Transform Transform::Combine(Transform const &Transform1, Transform const &Transform2)
{
	vec3 ResultLocation(
			Transform1.Location.x + Transform2.Location.x,
			Transform1.Location.y + Transform2.Location.y,
			Transform1.Location.z + Transform2.Location.z
	);
	vec3 ResultRotation(
			Transform1.Rotation.x + Transform2.Rotation.x,
			Transform1.Rotation.y + Transform2.Rotation.y,
			Transform1.Rotation.z + Transform2.Rotation.z
	);
	vec3 ResultScale(
			Transform1.Scale.x * Transform2.Scale.x,
			Transform1.Scale.y * Transform2.Scale.y,
			Transform1.Scale.z * Transform2.Scale.z
	);
	return Transform(ResultLocation, ResultRotation, ResultScale);
}

Transform Transform::Negate(Transform const & TransformToNegate)
{
	vec3 ResultLocation(
		-TransformToNegate.Location.x,
		-TransformToNegate.Location.y,
		-TransformToNegate.Location.z
	);
	vec3 ResultRotation(
		-TransformToNegate.Rotation.x,
		-TransformToNegate.Rotation.y,
		-TransformToNegate.Rotation.z
	);
	vec3 ResultScale(
		1 / TransformToNegate.Scale.x,
		1 / TransformToNegate.Scale.y,
		1 / TransformToNegate.Scale.z
	);
	return Transform(ResultLocation, ResultRotation, ResultScale);
}

std::string Transform::toString()
{
	std::string locationString("\tLocation: "+glm::to_string(Location)+"\n");
	std::string rotationString("\tRotation: "+glm::to_string(Rotation)+"\n");
	std::string scaleString("\tScale: "+glm::to_string(Scale)+"\n");
	return std::string("Transform:\n")+locationString+rotationString+scaleString;
}