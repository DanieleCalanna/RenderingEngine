#include "Core/Scene/SkyBox.hpp"
#include "Core/Camera/Camera.hpp"
#include <iostream>
#include "Utils/Png/lodepng.h"
#include "Core/Shaders/SkyBoxShader.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Transform.hpp"

void SkyBoxComponent::Start()
{
	float Scale = 1000.0f;
	float points[] = {
		-Scale,  Scale, -Scale,
		-Scale, -Scale, -Scale,
		Scale, -Scale, -Scale,
		Scale, -Scale, -Scale,
		Scale,  Scale, -Scale,
		-Scale,  Scale, -Scale,

		-Scale, -Scale,  Scale,
		-Scale, -Scale, -Scale,
		-Scale,  Scale, -Scale,
		-Scale,  Scale, -Scale,
		-Scale,  Scale,  Scale,
		-Scale, -Scale,  Scale,

		Scale, -Scale, -Scale,
		Scale, -Scale,  Scale,
		Scale,  Scale,  Scale,
		Scale,  Scale,  Scale,
		Scale,  Scale, -Scale,
		Scale, -Scale, -Scale,

		-Scale, -Scale,  Scale,
		-Scale,  Scale,  Scale,
		Scale,  Scale,  Scale,
		Scale,  Scale,  Scale,
		Scale, -Scale,  Scale,
		-Scale, -Scale,  Scale,

		-Scale,  Scale, -Scale,
		Scale,  Scale, -Scale,
		Scale,  Scale,  Scale,
		Scale,  Scale,  Scale,
		-Scale,  Scale,  Scale,
		-Scale,  Scale, -Scale,

		-Scale, -Scale, -Scale,
		-Scale, -Scale,  Scale,
		Scale, -Scale, -Scale,
		Scale, -Scale, -Scale,
		-Scale, -Scale,  Scale,
		Scale, -Scale,  Scale
	};
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	CreateCubeMap("Resources/Textures/CubeMap/lnegz.png",
				  "Resources/Textures/CubeMap/lposz.png",
				  "Resources/Textures/CubeMap/lposy.png",
				  "Resources/Textures/CubeMap/lnegy.png",
				  "Resources/Textures/CubeMap/lnegx.png",
				  "Resources/Textures/CubeMap/lposx.png"
	);
}

void SkyBoxComponent::CreateCubeMap(const std::string front, const std::string back, const std::string top, const std::string bottom, const std::string left, const std::string right)
{
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex_cube);

	// load each image and copy into a side of the cube-map texture
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool SkyBoxComponent::LoadCubeMapSide(GLenum side_target, const std::string TexturePath)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, TexturePath);

	// If there's an error, display it.
	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		exit(1);
	}
	// non-power-of-2 dimensions check
	if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0)
	{
		std::cout << "WARNING: image " << TexturePath << " is not power-of-2 dimensions" << std::endl;
	}
	glTexImage2D(side_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	// copy image data into 'target' side of cube map
	return true;
}

void SkyBoxComponent::Update()
{
	const Camera* ActiveCamera = Camera::GetActiveCamera();
	if (ActiveCamera)
	{
		glDepthMask(GL_FALSE);
		SkyBoxShader::GetInstance().Start();
		SkyBoxShader::GetInstance().LoadProjectionMatrix(ActiveCamera->GetProjectionMatrix());
		SkyBoxShader::GetInstance().LoadViewMatrix(ActiveCamera->GetViewMatrix());
		SkyBoxShader::GetInstance().LoadTransformationMatrix(Transform(ActiveCamera->GetWorldTransform().Location, glm::vec3(0), glm::vec3(1)).GetTransformationMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		SkyBoxShader::GetInstance().Stop();
	}
}
