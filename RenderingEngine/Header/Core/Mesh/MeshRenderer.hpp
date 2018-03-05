#pragma once

#include <Core/Components/Component.hpp>
#include <Core/Mesh/Mesh.hpp>
#include <Core/Shaders/StandardShader.hpp>

class MeshRenderer : public Component {
private:
	
public:
	Mesh* mesh;
	
	bool castShadow = false;
	bool receiveShadow = false;
	
	MeshRenderer(){}
	MeshRenderer(Mesh* mesh){
		this->mesh = mesh;
	}

	void SetMesh(Mesh* mesh){
		this->mesh = mesh;
	}
	
	void Start(){}
	
	void Update();

	void Clear(){
		mesh->Clear();
		delete mesh;
	}
};