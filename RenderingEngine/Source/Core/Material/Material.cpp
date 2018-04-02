#include "Core/Material/Material.hpp"
#include "Core/Texture/Texture.hpp"
#include "Utils/Tga.h"
#include <thread>         // std::thread

struct TextureLoadingThreadStruct
{
	Tga** TextureTga;
	std::string Path;
};

void Load(TextureLoadingThreadStruct TextureLoadingThreadParams)
{
	*TextureLoadingThreadParams.TextureTga = new Tga(TextureLoadingThreadParams.Path.c_str());
}

Material::Material(std::string AlbedoPath, std::string SpecularPath, std::string RoughnessPath, std::string NormalPath, std::string AOPath)
{
	//TO-DO check that TGA class load the file efficiently

	Tga* AlbedoTga = nullptr;
	Tga* SpecularTga = nullptr;
	Tga* RoughnessTga = nullptr;
	Tga* NormalTga = nullptr;
	Tga* AOTga = nullptr;

	TextureLoadingThreadStruct TextureLoadingThreadParams;

	TextureLoadingThreadParams.TextureTga = &AlbedoTga;
	TextureLoadingThreadParams.Path = AlbedoPath;
	std::thread AlbedoThread(Load, TextureLoadingThreadParams);

	TextureLoadingThreadParams.TextureTga = &SpecularTga;
	TextureLoadingThreadParams.Path = SpecularPath;
	std::thread SpecularThread(Load, TextureLoadingThreadParams);

	TextureLoadingThreadParams.TextureTga = &RoughnessTga;
	TextureLoadingThreadParams.Path = RoughnessPath;
	std::thread RoughnessThread(Load, TextureLoadingThreadParams);

	TextureLoadingThreadParams.TextureTga = &NormalTga;
	TextureLoadingThreadParams.Path = NormalPath;
	std::thread NormalThread(Load, TextureLoadingThreadParams);

	TextureLoadingThreadParams.TextureTga = &AOTga;
	TextureLoadingThreadParams.Path = AOPath;
	std::thread AOThread(Load, TextureLoadingThreadParams);

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
