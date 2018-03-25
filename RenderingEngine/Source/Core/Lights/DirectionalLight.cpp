#include "Core/Lights/DirectionalLight.hpp"
#include "Core/Transform.hpp"

DirectionalLight::DirectionalLight(std::string ActorName) : Actor(ActorName) {}

const glm::vec3 DirectionalLight::GetColor() const { return Color; }
