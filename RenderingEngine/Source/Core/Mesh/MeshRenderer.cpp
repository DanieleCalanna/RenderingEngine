#include "Core/Mesh/MeshRenderer.hpp"
#include "Core/Entities/Actor.hpp"
#include "Core/Transform.hpp"
#include "Core/Camera/Camera.hpp"
#include "Core/Lights/DirectionalLight.hpp"
#include "glm/vec3.hpp"

void MeshRenderer::SetMesh(Mesh * Mesh)
{
	MeshToRender = Mesh;
}

void MeshRenderer::Update()
{
	const Camera* ActiveCamera = Camera::GetActiveCamera();
	if (MeshToRender && ActiveCamera)
	{
		StandardShader::GetInstance().Start();
		StandardShader::GetInstance().LoadTransformationMatrix(GetOwner()->GetWorldTransform().GetTransformationMatrix());
		StandardShader::GetInstance().LoadProjectionMatrix(ActiveCamera->GetProjectionMatrix());
		StandardShader::GetInstance().LoadViewMatrix(ActiveCamera->GetViewMatrix());
		StandardShader::GetInstance().LoadDirectionalLight(DirectionalLight::GetSingletonInstance());
				
		//Solid + Wireframe
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);

		StandardShader::GetInstance().LoadColor(glm::vec3(0.75f, 0.75f, 0.75f));
		MeshToRender->Render();
		
		glDisable(GL_POLYGON_OFFSET_FILL);
		
		glPolygonOffset(1, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		StandardShader::GetInstance().LoadColor(glm::vec3(0.05f, 0.05f, 0.05f));
		MeshToRender->Render();
		
		StandardShader::GetInstance().Stop();
	}
}

void MeshRenderer::Clear()
{
	if (MeshToRender)
	{
		MeshToRender->Clear();
		delete MeshToRender;
	}
}
