#include <Core/Mesh/MeshRenderer.hpp>
#include <Core/Components/CameraComponent.hpp>
#include <Core/Lights/DirectionalLight.hpp>
#include <glm/vec3.hpp>
#include <iostream>

void MeshRenderer::Update(){
	if (mesh) {
		StandardShader::getInstance().start();
		StandardShader::getInstance().loadTransformationMatrix(gameObject->GetWorldTransform().GetTransformationMatrix());
		StandardShader::getInstance().loadProjectionMatrix(CameraComponent::getInstance().GetProjectionMatrix());
		StandardShader::getInstance().loadViewMatrix(CameraComponent::getInstance().GetViewMatrix());
		StandardShader::getInstance().loadDirectionalLight(DirectionalLight::getInstance());
				
		//Solid + Wireframe
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);

		StandardShader::getInstance().loadColor(glm::vec3(0.75f, 0.75f, 0.75f));
		mesh->Render();
		
		glDisable(GL_POLYGON_OFFSET_FILL);
		/*
		glPolygonOffset(1, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		StandardShader::getInstance().loadColor(glm::vec3(0.05f, 0.05f, 0.05f));
		mesh->Render();
		*/
		
		StandardShader::getInstance().stop();
	}
}