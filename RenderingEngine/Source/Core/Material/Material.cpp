#include "Core/Material/Material.hpp"
#include "Core/Texture/Texture.hpp"
#include "Utils/Tga.h"
#include <thread>         // std::thread

struct Aei
{
	Tga** TextureTga;
	std::string Path;
};

void Load(Aei AeiAei)
{
	*AeiAei.TextureTga = new Tga(AeiAei.Path.c_str());
}

Material::Material(std::string AlbedoPath, std::string SpecularPath, std::string RoughnessPath, std::string NormalPath, std::string AOPath)
{
	Tga* AlbedoTga = nullptr;
	Tga* SpecularTga = nullptr;
	Tga* RoughnessTga = nullptr;
	Tga* NormalTga = nullptr;
	Tga* AOTga = nullptr;

	Aei AeiAei;

	AeiAei.TextureTga = &AlbedoTga;
	AeiAei.Path = AlbedoPath;
	std::thread AlbedoThread(Load, AeiAei);

	AeiAei.TextureTga = &SpecularTga;
	AeiAei.Path = SpecularPath;
	std::thread SpecularThread(Load, AeiAei);

	AeiAei.TextureTga = &RoughnessTga;
	AeiAei.Path = RoughnessPath;
	std::thread RoughnessThread(Load, AeiAei);

	AeiAei.TextureTga = &NormalTga;
	AeiAei.Path = NormalPath;
	std::thread NormalThread(Load, AeiAei);

	AeiAei.TextureTga = &AOTga;
	AeiAei.Path = AOPath;
	std::thread AOThread(Load, AeiAei);


	AlbedoThread.join();
	Albedo = new Texture(*AlbedoTga);

	SpecularThread.join();
	Specular = new Texture(*SpecularTga);

	RoughnessThread.join();
	Roughness = new Texture(*RoughnessTga);

	NormalThread.join();
	Normal = new Texture(*NormalTga);

	AOThread.join();
	AO = new Texture(*AOTga);


	delete AlbedoTga;
	delete SpecularTga;
	delete RoughnessTga;
	delete NormalTga;
	delete AOTga;
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
