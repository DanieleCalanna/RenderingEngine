#pragma once

#include "Core/Components/Component.hpp"
#include "GL/glew.h"
#include <string>

class SkyBoxComponent : public Component
{
	GLuint CubeVertexArray;
	GLuint CubeVertexBuffer;
	GLuint CubeTexture;

public:

	virtual void Start() override;
	virtual void Update() override;

private:

	void CreateCubeMap(
		const std::string Front,
		const std::string Back,
		const std::string Top,
		const std::string Bottom,
		const std::string Left,
		const std::string Right);

	bool LoadCubeMapSide(GLenum side_target, const std::string TexturePath);

	

};