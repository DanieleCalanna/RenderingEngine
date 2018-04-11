#pragma once

#include "Core/Components/Component.hpp"
#include "GL/glew.h"
#include <string>

class SkyBoxComponent : public Component
{
	GLuint CubeVertexArray;
	GLuint CubeVertexBuffer;

	GLuint CubeTexture;
	GLuint HdrTexture;

public:
	virtual void Construct() override;
	virtual void Start() override;
	virtual void Update() override;

	GLuint GetCubeMapTextureId() const;

private:

	void CreateCubeMap(
		const std::string Front,
		const std::string Back,
		const std::string Top,
		const std::string Bottom,
		const std::string Left,
		const std::string Right);

	bool LoadCubeMapSide(GLenum side_target, const std::string TexturePath);

	GLuint GetCubemapFromHdr(std::string HdrPath, GLsizei Size) const;
};