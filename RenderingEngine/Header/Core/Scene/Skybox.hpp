#pragma once

#include "Core/Components/Component.hpp"
#include "GL/glew.h"
#include <string>

class SkyBoxComponent : public Component
{
	GLuint CubeVertexArray;
	GLuint CubeVertexBuffer;

	GLuint QuadVertexArray;
	GLuint QuadVertexBuffer;

	GLuint CubemapTexture;
	GLuint IrradianceCubemap;
	GLuint HdrTexture;
	GLuint PrefilteredCubemap;
	GLuint BRDFTexture;

public:

	SkyBoxComponent(std::string HdrMapPath, GLsizei HdrMapSize);

	virtual void Construct() override;
	virtual void Start() override;
	virtual void Update() override;

	GLuint GetCubemapTextureId() const;
	GLuint GetIrradianceMapTextureId() const;
	GLuint GetPrefilteredCubemapTextureId() const;
	GLuint GetBRDFTextureId() const;

	GLuint GetConvolutedCubemap(GLuint Cubemap, GLsizei Size) const;
	GLuint GetCubemapFromEquirectangular(std::string HdrPath, GLsizei Size) const;
	GLuint GetPrefilteredCubemap(GLuint Cubemap, GLsizei Size) const;
	GLuint GetBRDFTexture(GLsizei Size) const;

private:

	void CreateCubemap(
		const std::string Front,
		const std::string Back,
		const std::string Top,
		const std::string Bottom,
		const std::string Left,
		const std::string Right);

	bool LoadCubemapSide(GLenum side_target, const std::string TexturePath);

};


