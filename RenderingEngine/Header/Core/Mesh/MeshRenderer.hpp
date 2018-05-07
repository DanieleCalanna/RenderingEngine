#pragma once

#include <Core/Components/Component.hpp>
#include <Core/Mesh/Mesh.hpp>
#include <Core/Shaders/StandardShader.hpp>

class Mesh;
class Material;

class MeshRenderer : public Component 
{
private:

	Mesh* MeshToRender = nullptr;
	Material* MaterialToRender = nullptr;

	bool bCastShadow = false;
	bool bReceiveShadow = false;

public:

	MeshRenderer() : Component("MeshRenderer") {}

	void SetMesh(Mesh* InMesh);
	void SetMaterial(Material* InMaterial);
	
	void Start(){}
	
	void Update();

	void Clear();

	Mesh* GetMesh() const;
};