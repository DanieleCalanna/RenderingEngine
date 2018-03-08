#pragma once

#include <Core/Components/Component.hpp>
#include <Core/Mesh/Mesh.hpp>
#include <Core/Shaders/StandardShader.hpp>

class Mesh;

class MeshRenderer : public Component 
{
private:

	Mesh* MeshToRender = nullptr;

	bool bCastShadow = false;
	bool bReceiveShadow = false;

public:

	void SetMesh(Mesh* Mesh); 
	
	void Start(){}
	
	void Update();

	void Clear();
};