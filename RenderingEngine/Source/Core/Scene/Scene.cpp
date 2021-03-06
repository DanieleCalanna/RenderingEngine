#include "Core/Scene/Scene.hpp"
#include "Core/Scene/Skybox.hpp"

Scene* Scene::CurrentScene = nullptr;

Scene::Scene() : Actor("Scene")
{
	//SkyBox = AddComponent<SkyBoxComponent>("SkyBox");
	//SkyBox->SetEnabled(false);
}

Scene::~Scene()
{
}

void Scene::SetAsCurrentScene()
{
	CurrentScene = this;
}

Scene * Scene::GetCurrentScene()
{
	return CurrentScene;
}
