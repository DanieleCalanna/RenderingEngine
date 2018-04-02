#include "Core/Material/Material.hpp"
#include "Core/Texture/Texture.hpp"

Material::Material(std::string AlbedoPath, std::string SpecularPath, std::string RoughnessPath, std::string NormalPath, std::string AOPath)
{
	Albedo = new Texture(AlbedoPath);
	Specular = new Texture(SpecularPath);
	Roughness = new Texture(RoughnessPath);
	Normal = new Texture(NormalPath);
	AO = new Texture(AOPath);
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
