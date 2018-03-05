#include <GL/glew.h>

#include <vector>
#include <string>
#include <iostream>

#include "Core/Window.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Mesh/Mesh.hpp"
#include "Core/Mesh/IndexedMesh.hpp"
#include "Core/Entities/GameObject.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Components/CameraComponent.hpp"
#include "Core/Mesh/MeshRenderer.hpp"
#include "Core/Mesh/MeshSmoothing.hpp"
#include "Core/Components/CameraController.hpp"
#include "Utils/Axes.hpp"


int main(int argc, char** argv)
{

	Window::GetWindow().Run();

	return 0;
}


GameObject scene;
GameObject cameraObject;
MeshRenderer meshRenderer;
MeshSmoothing meshSmoothing;
GameObject meshGameObject;
CameraController cameraController;

void Init()
{
	/*-- Camera Start --*/
	//CameraController cameraController = new CameraController();
	cameraObject.transform.Location = glm::vec3(0.0f, 30.0f, 100.0f);
	//cameraObject.transform.rotation.Rotate(new Vector4f(1,0,0,(float)(22*Math.PI/180)));
	cameraObject.AddComponent(CameraComponent::getInstance());
	cameraObject.AddComponent(cameraController);
	scene.AddChild(cameraObject);
	//cameraController.Enabled = true;
	/*-- Camera End --*/

	/*-- Directional Light Start --*/
	scene.AddComponent(DirectionalLight::getInstance());
	/*-- Directional Light End --*/
	
	/*
	meshRenderer.mesh = IndexedMesh::FromOBJFile("res/OBJ/wrestlers.obj");

	meshGameObject.transform.Location = glm::vec3(0.0f, 0.0f, 0.0f);
	meshGameObject.transform.Scale = glm::vec3(5.0f, 5.0f, 5.0f);
	meshGameObject.AddComponent(meshRenderer);
	meshGameObject.AddComponent(meshSmoothing);
	
	scene.AddChild(meshGameObject);
	*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	scene.Start();
}

void Loop()
{
	//meshGameObject.transform.Rotation.y+=1;
	Axes::glLoadCameraMatrix();
	Axes::glDrawGrid();
	Axes::glDrawAxes();
	scene.Update();
}

void Clear()
{

}