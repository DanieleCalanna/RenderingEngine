#include "Core/Window.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Mesh/Mesh.hpp"
#include "Core/Material/Material.hpp"
#include "Core/Mesh/IndexedMesh.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Camera/CameraComponent.hpp"
#include "Core/Mesh/MeshRenderer.hpp"
#include "Core/Mesh/MeshSmoothing.hpp"
#include "Core/Camera/Camera.hpp"
#include "Core/Transform.hpp"
#include "Utils/Axes.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <iostream>

#ifndef GLFW_KEY_R
#define GLFW_KEY_R   82
#endif

Scene* MainScene;
Camera* MainCamera;
Actor* ActorMeshTest;
Shader* ShaderTest;
Mesh* MeshTest;

void Init()
{
	MainScene = new Scene();
	MainScene->SetAsCurrentScene();

	/*-- Camera Start --*/
	MainCamera = new Camera("Camera");
	MainCamera->AttachToActor(MainScene);
	MainCamera->SetActive();
	Transform CameraTransform;
	CameraTransform.Location = glm::vec3(0.0f, 0.0f, 3.0f);
	//CameraTransform.Location = glm::vec3(15.0f, 2.0f, 9.0f);
	MainCamera->SetWorldTransform(CameraTransform);
	/*-- Camera End --*/

	/*-- Directional Light --*/
	DirectionalLight::GetSingletonInstance().SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(4.3f, 0.5f, -0.5f)), glm::vec3(1.0f, 1.0f, 1.0f)));
	
	
	MeshTest = new Mesh("Resources/3DObj/Sphere.3Dobj");

	/*
	MeshTest = new Mesh("Resources/3DObj/Gun.3Dobj");
	Material* MaterialTest = new Material(
		"Resources/Textures/Gun/Cerberus_A.tga",
		"Resources/Textures/Gun/Cerberus_M.tga",
		"Resources/Textures/Gun/Cerberus_R.tga",
		"Resources/Textures/Gun/Cerberus_N.tga",
		"Resources/Textures/Gun/Cerberus_AO.tga");
	ActorMeshTest = new Actor("Gun");
	*/

	//MeshTest = new IndexedMesh("Resources/3DObj/Drone.3Dobj");
	ShaderTest = new Shader("Resources/Shaders/BaseColorShader/BaseColorVertex.glsl", "Resources/Shaders/BaseColorShader/BaseColorFragment.glsl");
	ShaderTest->Refresh();

	/*
	MeshTest = new Mesh("Resources/3DObj/Drone.3Dobj");
	Material* MaterialTest = new Material(
		"Resources/Textures/Drone/Drone_diff.jpg",
		"Resources/Textures/Drone/Drone_spec.jpg",
		"Resources/Textures/Drone/Drone_gloss.jpg",
		"Resources/Textures/Drone/Drone_normal.jpg",
		"Resources/Textures/Drone/Drone_ao.jpg");
	ActorMeshTest = new Actor("Drone");
	*/
	/*
	Transform ActorMeshTestTransform;
	ActorMeshTestTransform.Location = glm::vec3(0.0f, 0.0f, 0.0f);
	ActorMeshTestTransform.Scale = glm::vec3(20.0f, 20.0f, 20.0f);
	ActorMeshTest->SetWorldTransform(ActorMeshTestTransform);
	MeshRenderer* MeshTestRenderer = ActorMeshTest->AddComponent<MeshRenderer>("GunMeshRenderer");
	MeshTestRenderer->SetMesh(MeshTest);
	MeshTestRenderer->SetMaterial(MaterialTest);
	//ActorMeshTest->AddComponent<MeshSmoothing>("GunMeshSmoothing");
	ActorMeshTest->AttachToActor(MainScene);
	*/
	
	MainScene->Start();
}

void Loop()
{
	/*
	auto ActorTransform = ActorMeshTest->GetWorldTransform();
	ActorTransform.Rotation = glm::rotate(ActorTransform.Rotation, 0.001f, glm::vec3(0.0f, 0.0f, -1.0f));
	ActorMeshTest->SetWorldTransform(ActorTransform);
	*/

	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_R))
	{
		StandardShader::GetInstance().Refresh();
		ShaderTest->Refresh();
	}

	
	ShaderTest->Start();
	ShaderTest->LoadMat4("TransformationMatrix", MainScene->GetWorldTransform().GetTransformationMatrix());
	ShaderTest->LoadMat4("ProjectionMatrix", MainCamera->GetProjectionMatrix());
	ShaderTest->LoadMat4("ViewMatrix", MainCamera->GetViewMatrix());
	MeshTest->Render();
	ShaderTest->Stop();
	

	//std::cout << "Framerate : " << 1.0f/Window::GetSingletonWindow().GetDeltaTime() << std::endl;

	//Axes::LoadCameraMatrix();
	//Axes::DrawGrid();
	//Axes::DrawAxes();
	MainScene->Update();
}

void Clear()
{
	MainScene->Clear();
}

int main(int argc, char** argv)
{
	Window::GetSingletonWindow().SetInitFunction(Init);
	Window::GetSingletonWindow().SetLoopFunction(Loop);
	Window::GetSingletonWindow().SetClearFunction(Clear);
	Window::GetSingletonWindow().Run();
	return 0;
}
