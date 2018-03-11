#include "GL/glew.h"
#include "Core/Window.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Mesh/Mesh.hpp"
#include "Core/Mesh/IndexedMesh.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Camera/CameraComponent.hpp"
#include "Core/Mesh/MeshRenderer.hpp"
#include "Core/Mesh/MeshSmoothing.hpp"
#include "Core/Camera/Camera.hpp"
#include "Core/Transform.hpp"
#include "Utils/Axes.hpp"

#include <vector>
#include <string>
#include <iostream>


Scene* MainScene;

void Init()
{
	MainScene = new Scene();
	/*-- Camera Start --*/
	Camera* MainCamera = new Camera("Camera");
	MainCamera->AttachToActor(MainScene);
	MainCamera->SetActive();
	Transform CameraTransform;
	CameraTransform.Location = glm::vec3(0.0f, 30.0f, 100.0f);
	CameraTransform.Rotation = glm::vec3(22*3.14f/180, 0.0f, 0.0f);
	MainCamera->SetWorldTransform(CameraTransform);
	/*-- Camera End --*/

	/*-- Directional Light Start --*/
	//DirectionalLight* NewDirectionalLight = new DirectionalLight("DirectionalLight");
	//NewDirectionalLight->SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100, -300, 1), glm::vec3(1.0f, 1.0f, 1.0f)));
	DirectionalLight::GetSingletonInstance().SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100, -300, 1), glm::vec3(1.0f, 1.0f, 1.0f)));
	/*-- Directional Light End --*/
	
	Mesh* MeshTest = new Mesh("Resources/Obj/gun.obj");
	Actor* ActorMeshTest = new Actor("Gun");
	Transform ActorMeshTestTransform;
	ActorMeshTestTransform.Location = glm::vec3(0.0f, 0.0f, 0.0f);
	ActorMeshTestTransform.Scale = glm::vec3(20.0f, 20.0f, 20.0f);
	ActorMeshTest->SetWorldTransform(ActorMeshTestTransform);
	MeshRenderer* MeshTestRenderer = ActorMeshTest->AddComponent<MeshRenderer>();
	MeshTestRenderer->SetMesh(MeshTest);
	ActorMeshTest->AttachToActor(MainScene);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	MainScene->Start();
}

void Loop()
{
	//meshGameObject.transform.Rotation.y+=1;

	const Camera* ActiveCamera = Camera::GetActiveCamera();

	Axes::LoadCameraMatrix();
	Axes::DrawGrid();
	Axes::DrawAxes();
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
