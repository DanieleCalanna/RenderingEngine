#pragma once

#include "GL\glew.h"
#include <string>

class Texture;

class Material
{
public:
	Material(std::string AlbedoPath, std::string MetallicPath, std::string RoughnessPath, std::string NormalPath, std::string AOPath);

	void Activate(GLenum AlbedoSlot, GLenum SpecularSlot, GLenum RoughnessSlot, GLenum NormalSlot, GLenum AOSlot);
	void Deactivate();

private:
	Texture* Albedo = nullptr;
	Texture* Metallic = nullptr;
	Texture* Roughness = nullptr;
	Texture* Normal = nullptr;
	Texture* AO = nullptr;
};