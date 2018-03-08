#include <GL/glew.h>

#include <vector>
#include <string>
#include <iostream>

#include "Core/Window.hpp"
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

Actor* Scene;

void Init()
{
	/*-- Camera Start --*/
	Scene = new Actor();
	Camera* MainCamera = new Camera();
	MainCamera->AttachToActor(Scene);
	MainCamera->SetActive();
	Transform CameraTransform;
	CameraTransform.Location = glm::vec3(0.0f, 30.0f, 100.0f);
	//CameraTransform.Rotation.Rotate(new Vector4f(1,0,0,(float)(22*Math.PI/180)));
	MainCamera->SetWorldTransform(CameraTransform);
	/*-- Camera End --*/

	/*-- Directional Light Start --*/
	Actor* NewDirectionalLight = new Actor();
	NewDirectionalLight->AddComponent<DirectionalLight>();
	/*-- Directional Light End --*/
	
	Mesh* MeshTest = new Mesh("Resources/Obj/gun.obj");
	Actor* ActorMeshTest = new Actor();
	Transform ActorMeshTestTransform;
	ActorMeshTestTransform.Location = glm::vec3(0.0f, 0.0f, 0.0f);
	ActorMeshTestTransform.Scale = glm::vec3(5.0f, 5.0f, 5.0f);
	ActorMeshTest->SetWorldTransform(ActorMeshTestTransform);
	MeshRenderer* MeshTestRenderer = ActorMeshTest->AddComponent<MeshRenderer>();
	MeshTestRenderer->SetMesh(MeshTest);
	ActorMeshTest->AttachToActor(Scene);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Scene->Start();
}

void Loop()
{
	//meshGameObject.transform.Rotation.y+=1;
	Axes::LoadCameraMatrix();
	Axes::DrawGrid();
	Axes::DrawAxes();
	Scene->Update();
}

void Clear()
{
	Scene->Clear();
}


int main(int argc, char** argv)
{
	Window::GetWindow().Run();
	return 0;
}
