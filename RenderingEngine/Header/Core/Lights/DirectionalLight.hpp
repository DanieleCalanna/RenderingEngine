#pragma once

#include <Core/Entities/Actor.hpp>
#include <glm/vec3.hpp>

class DirectionalLight : public Actor
{
public:

	DirectionalLight(std::string ActorName);

	const glm::vec3 GetColor() const;
	const glm::vec3 GetDirection() const;

private:
	glm::vec3 Color = glm::vec3(1.0f);
	bool bGeneratesShadows = true;
};