#include "Core/Material/Material.hpp"
#include "Core/Texture/Texture.hpp"
#include "Utils/Tga.h"
#include <thread>

Material::Material(std::string AlbedoPath, std::string SpecularPath, std::string RoughnessPath, std::string NormalPath, std::string AOPath)
{
	Albedo = new Texture(AlbedoPath, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	Specular = new Texture(SpecularPath, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	Roughness = new Texture(RoughnessPath, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	Normal = new Texture(NormalPath, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	AO = new Texture(AOPath, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}

void Material::Activate(GLenum AlbedoSlot, GLenum SpecularSlot, GLenum RoughnessSlot, GLenum NormalSlot, GLenum AOSlot)
{
	Albedo->Activate(GL_TEXTURE0);
	Specular->Activate(GL_TEXTURE1);
	Roughness->Activate(GL_TEXTURE2);
	Normal->Activate(GL_TEXTURE3);
	AO->Activate(GL_TEXTURE4);
}

void Material::Deactivate()
{
	Albedo->Deactivate();
	Specular->Deactivate();
	Roughness->Deactivate();
	Normal->Deactivate();
	AO->Deactivate();
}
