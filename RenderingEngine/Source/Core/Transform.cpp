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


Transform Transform::Combine(Transform const &transform1, Transform const &transform2)
{
	vec3 location(
			transform1.Location.x + transform2.Location.x,
			transform1.Location.y + transform2.Location.y,
			transform1.Location.z + transform2.Location.z);
		
	vec3 rotation(
			transform1.Rotation.x + transform2.Rotation.x,
			transform1.Rotation.y + transform2.Rotation.y,
			transform1.Rotation.z + transform2.Rotation.z);

	vec3 scale(
			transform1.Scale.x * transform2.Scale.x,
			transform1.Scale.y * transform2.Scale.y,
			transform1.Scale.z * transform2.Scale.z);
	return Transform(location, rotation, scale);
}

std::string Transform::toString()
{
	std::string locationString("\tLocation: "+glm::to_string(Location)+"\n");
	std::string rotationString("\tRotation: "+glm::to_string(Rotation)+"\n");
	std::string scaleString("\tScale: "+glm::to_string(Scale)+"\n");
	return std::string("Transform:\n")+locationString+rotationString+scaleString;
}