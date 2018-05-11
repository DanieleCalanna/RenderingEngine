#include "Core/Window.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Mesh/Mesh.hpp"
#include "Core/Mesh/IndexedMesh.hpp"
#include "Core/Material/Material.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Camera/Camera.hpp"
#include "Core/Camera/CameraComponent.hpp"
#include "Core/Mesh/MeshRenderer.hpp"
#include "Core/Mesh/MeshSmoothing.hpp"
#include "Core/Transform.hpp"
#include "Core/Scene/SkyBox.hpp"
#include "Core/Texture/Texture.hpp"
#include "Core/GUI/GUI.hpp"
#include "Utils/Axes.hpp"
#include "AntTweakBar.h"
#include "GLFW/glfw3.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <algorithm>

Scene* MainScene;
SkyBoxComponent* SceneSkyBox;
Actor* CameraContainer;
Camera* MainCamera;
Actor* ActorMeshTest;
Shader* ShaderTest;
Mesh* MeshTest;

float Scale = 1.0f;
Transform MeshTransform;

glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 LightDirection = glm::vec3(-4.5f, -4.5f, -4.5f);
float LightIntensity = 1.0f;

glm::vec3 BaseColor = glm::vec3(1.0f, 1.0f, 1.0f);
float Roughness = 0.04f;
float Metalness = 0.3f;

MeshSmoothing* MeshSmoothingTest;
bool SmoothingEnabled = false;


#if 0
void Init()
{
	MainScene = new Scene();
	MainScene->SetAsCurrentScene();
	MainScene->AddComponent(new SkyBoxComponent("Resources/Textures/HDR/venice_sunset_4k.hdr", 4096));

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
	//DirectionalLight::GetSingletonInstance().SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(4.3f, 0.5f, -0.5f)), glm::vec3(1.0f, 1.0f, 1.0f)));
	DirectionalLight::GetSingletonInstance().SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::normalize(glm::vec3(-4.5f, -4.5f, -4.5f))), glm::vec3(1.0f, 1.0f, 1.0f)));


	
	MeshTest = new Mesh("Resources/3DObj/Gun.3Dobj");
	Material* MaterialTest = new Material(
	"Resources/Textures/Gun/Cerberus_A.tga",
	"Resources/Textures/Gun/Cerberus_M.tga",
	"Resources/Textures/Gun/Cerberus_R.tga",
	"Resources/Textures/Gun/Cerberus_N.tga",
	"Resources/Textures/Gun/Cerberus_AO.tga");
	ActorMeshTest = new Actor("Gun");
	

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
	
	Transform ActorMeshTestTransform;
	ActorMeshTestTransform.Location = glm::vec3(0.0f, 0.0f, 0.0f);
	ActorMeshTestTransform.Scale = glm::vec3(20.0f, 20.0f, 20.0f);
	ActorMeshTest->SetWorldTransform(ActorMeshTestTransform);
	MeshRenderer* MeshTestRenderer = ActorMeshTest->AddComponent<MeshRenderer>("GunMeshRenderer");
	MeshTestRenderer->SetMesh(MeshTest);
	MeshTestRenderer->SetMaterial(MaterialTest);
	//ActorMeshTest->AddComponent<MeshSmoothing>("GunMeshSmoothing");
	ActorMeshTest->AttachToActor(MainScene);
	

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
	}
	
	//std::cout << "Framerate : " << 1.0f/Window::GetSingletonWindow().GetDeltaTime() << std::endl;

	//Axes::LoadCameraMatrix();
	//Axes::DrawGrid();
	//Axes::DrawAxes();

	MainScene->Update();
	GUI::GetInstance().RenderText(std::to_string(int(1.0f / Window::GetSingletonWindow().GetDeltaTime())), 20.0f, 20.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
}
#else

void Init()
{
	MainScene = new Scene();
	MainScene->SetAsCurrentScene();
	SceneSkyBox = new SkyBoxComponent("Resources/Textures/HDR/venice_sunset_4k.hdr", 4096);
	SceneSkyBox->SetEnabled(true);
	MainScene->AddComponent(SceneSkyBox);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	/*-- Camera Start --*/
	CameraContainer = new Actor("CameraContainer");
	CameraContainer->AttachToActor(MainScene);
	CameraContainer->SetWorldTransform(Transform());
	MainCamera = new Camera("Camera");
	MainCamera->AttachToActor(CameraContainer);
	MainCamera->SetActive();
	Transform CameraTransform;
	CameraTransform.Location = glm::vec3(0.0f, 0.0f, 3.0f);
	MainCamera->SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f)));
	/*-- Camera End --*/

	//MeshTest = new Mesh("Resources/3DObj/Sphere.3Dobj");
	//MeshTest = new Mesh("Resources/3DObj/Bunny.3Dobj");
	Scale = 0.65f;
	
	MeshTest = new IndexedMesh("Resources/3DObj/Bunny.3Dobj");
	//MeshTransform.Location = glm::vec3(0.0f, -0.5f, 0.0f);
	

	ShaderTest = new Shader("Resources/Shaders/BaseColorShader/BaseColorVertex.glsl", "Resources/Shaders/BaseColorShader/BaseColorFragment.glsl");
	ShaderTest->Refresh();
	ShaderTest->Start();
	ShaderTest->LoadInt("IrradianceMap", 0);
	ShaderTest->LoadInt("PrefilterMap", 1);
	ShaderTest->LoadInt("BRDFLUT", 2);
	ShaderTest->Stop();

	ActorMeshTest = new Actor("ActorTest");
	MeshSmoothingTest = (MeshSmoothing*)ActorMeshTest->AddComponent(new MeshSmoothing((IndexedMesh*)MeshTest));
	ActorMeshTest->AttachToActor(MainScene);

	TwDefine(" GLOBAL contained=true help='OpenGL Rendering and Smoothing' ");
	TwBar* RenderingBar = TwNewBar("RenderingBar");
	TwDefine(" RenderingBar label='Rendering' position='16 16' size='220 240' help='Use this bar to edit the Rendering' ");

	TwAddVarRW(RenderingBar, "LightColor", TW_TYPE_COLOR3F, &LightColor[0], " label='Light color' ");
	TwAddVarRW(RenderingBar, "LightDir", TW_TYPE_DIR3F, &LightDirection[0], " label='Light direction' opened=true help='Change the light direction' ");
	TwAddVarRW(RenderingBar, "LightIntensity", TW_TYPE_FLOAT, &LightIntensity, " label='Light intensity' min=0 step=0.01 keyIncr=l keyDecr=L help='Light intensity' ");

	TwAddVarRW(RenderingBar, "BaseColor", TW_TYPE_COLOR3F, &BaseColor[0], " label='Base color' ");
	TwAddVarRW(RenderingBar, "Roughness", TW_TYPE_FLOAT, &Roughness, " label='Roughness' min=0 max=1 step=0.01 keyIncr=r keyDecr=R help='Roughness' ");
	TwAddVarRW(RenderingBar, "Metalness", TW_TYPE_FLOAT, &Metalness, " label='Metalness' min=0 max=1 step=0.01 keyIncr=m keyDecr=M help='Metalness' ");

	TwBar* ObjectBar = TwNewBar("ObjectBar");
	TwDefine(" ObjectBar label='Object' position='300 16' size='220 250' help='Use this bar to edit the Object' ");

	TwAddVarRW(ObjectBar, "ObjRotation", TW_TYPE_QUAT4F, &MeshTransform.Rotation[0], " label='Object rotation' opened=true help='Change the object orientation' ");
	TwAddVarRW(ObjectBar, "ObjScale", TW_TYPE_FLOAT, &Scale, " label='Object Scale' min=0 step=0.01 keyIncr=+ keyDecr=- help='Change the object Scale' ");
	//TwAddVarRW(ObjectBar, "ObjPan", TW_TYPE_DIR3F, &MeshTransform.Location[0], " label='Object Pan'  help='Change the object position' ");
	TwAddVarRW(ObjectBar, "ObjPanX", TW_TYPE_FLOAT, &MeshTransform.Location[0], " label='Pan x' step=0.01 keyIncr=d keyDecr=a help='Pan x' ");
	TwAddVarRW(ObjectBar, "ObjPanY", TW_TYPE_FLOAT, &MeshTransform.Location[1], " label='Pan y' step=0.01 keyIncr=w keyDecr=s help='Pan y' ");
	TwAddVarRW(ObjectBar, "ObjPanZ", TW_TYPE_FLOAT, &MeshTransform.Location[2], " label='Pan z' step=0.01 keyIncr=e keyDecr=q help='Pan z' ");


	if (MeshSmoothingTest)
	{
		TwBar* SmoothingBar = TwNewBar("SmoothingBar");
		TwDefine(" SmoothingBar label='Smoothing' position='16 370' help='Use this bar to edit the Smoothing' refresh=0.0 ");
		TwAddVarRW(SmoothingBar, "Lambda", TW_TYPE_FLOAT, &MeshSmoothingTest->lambda, " label='Lambda' min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R help='Roughness' ");
		TwAddVarRW(SmoothingBar, "Mu", TW_TYPE_FLOAT, &MeshSmoothingTest->mi, " label='Mu' min=-1.0 max=1.0 step=0.01 keyIncr=m keyDecr=M help='Metalness' ");
		TwAddVarRW(SmoothingBar, "IterationsPerFrame", TW_TYPE_INT32, &MeshSmoothingTest->iterationsPerFrame, " label='Iterations/Frame' min=1 max=1000 step=1 help='Iterations per frame' ");
		TwAddVarRO(SmoothingBar, "Iterations", TW_TYPE_INT32, &MeshSmoothingTest->iterationsCounter, " label='Iterations' help='Iterations' ");

		TwAddVarRW(SmoothingBar, "SmoothingEnabled", TW_TYPE_BOOL32, &SmoothingEnabled, " label='Smoothing' true='Enabled' false='Disabled' key='' help='Enable/Disable Smoothing' ");
	}

	MainScene->Start();
}

void Loop()
{
	
	if (Window::GetSingletonWindow().GetKeyDown(GLFW_KEY_R))
	{
		ShaderTest->Refresh();
		ShaderTest->Start();
		ShaderTest->LoadInt("IrradianceMap", 0);
		ShaderTest->LoadInt("PrefilterMap", 1);
		ShaderTest->LoadInt("BRDFLUT", 2);
		ShaderTest->Stop();
	}

	//CameraContainer->SetWorldTransform(Transform(glm::vec3(0.0f, 0.0f, 0.0f), MeshTransform.Rotation, glm::vec3(1.0f, 1.0f, 1.0f)));
	if (MeshSmoothingTest)
	{
		MeshSmoothingTest->SetEnabled(SmoothingEnabled);
	}
	MeshTransform.Scale = glm::vec3(Scale, Scale, Scale);
	ShaderTest->Start();
	ShaderTest->LoadMat4("TransformationMatrix", MeshTransform.GetTransformationMatrix());
	ShaderTest->LoadMat4("ProjectionMatrix", MainCamera->GetProjectionMatrix());
	ShaderTest->LoadMat4("ViewMatrix", MainCamera->GetViewMatrix());
	ShaderTest->LoadFloat3("LightColor", LightColor);
	ShaderTest->LoadFloat3("LightDirection", LightDirection);
	ShaderTest->LoadFloat("LightIntensity", LightIntensity);
	ShaderTest->LoadFloat3("BaseColor", BaseColor);
	ShaderTest->LoadFloat("Roughness", Roughness);
	ShaderTest->LoadFloat("Metalness", Metalness);
	
	if(SceneSkyBox)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SceneSkyBox->GetIrradianceMapTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SceneSkyBox->GetPrefilteredCubemapTextureId());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, SceneSkyBox->GetBRDFTextureId());
	}

	MeshTest->Render();
	ShaderTest->Stop();
	
	Axes::LoadCameraMatrix();
	Axes::DrawGrid();
	Axes::DrawAxes();

	MainScene->Update();
	//GUI::GetInstance().RenderText(std::to_string(int(1.0f / Window::GetSingletonWindow().GetDeltaTime())), 20.0f, 20.0f, 0.6f, glm::vec3(0.5, 0.8f, 0.2f));
}
#endif

void Clear()
{
	MainScene->Clear();
}
int main(int argc, char* argv[])
{
	Window::GetSingletonWindow().SetInitFunction(Init);
	Window::GetSingletonWindow().SetLoopFunction(Loop);
	Window::GetSingletonWindow().SetClearFunction(Clear);
	Window::GetSingletonWindow().Run();
	return 0;
}


