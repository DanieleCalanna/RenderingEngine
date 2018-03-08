#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>

using namespace glm;

class Transform {
	public:
		vec3 Location = vec3(0.0f);
		vec3 Rotation = vec3(0.0f);
		vec3 Scale = vec3(1.0f);
	
		mat4x4 transformationMatrix = mat4x4(1.0f);
	
	Transform();
	~Transform();

	Transform(vec3 Location, vec3 Rotation, vec3 Scale);

	mat4x4 GetTransformationMatrix();
	
	static Transform Combine(Transform const &Transform1, Transform const &Transform2);
	static Transform Negate(Transform const &TransformToNegate);

	std::string toString();
};
