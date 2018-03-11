#include "Core/Scene/Scene.hpp"
#include "Core/Scene/Skybox.hpp"

Scene::Scene() : Actor("Scene")
{
	SkyBox = AddComponent<SkyBoxComponent>();
}

Scene::~Scene()
{
}
