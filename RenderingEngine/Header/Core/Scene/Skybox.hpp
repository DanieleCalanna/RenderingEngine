#pragma once

#include "Core/Components/Component.hpp"
#include "GL/glew.h"
#include <string>

class SkyBoxComponent : public Component
{
	GLuint vao;
	GLuint vbo;
	GLuint tex_cube;

public:

	virtual void Start() override;

	void CreateCubeMap(
		const std::string front,
		const std::string back,
		const std::string top,
		const std::string bottom,
		const std::string left,
		const std::string right);

	bool LoadCubeMapSide(GLenum side_target, const std::string TexturePath);

	virtual void Update() override;

};