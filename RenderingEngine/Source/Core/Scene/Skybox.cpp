#include "Core/Scene/SkyBox.hpp"
#include "Core/Camera/Camera.hpp"
#include <iostream>
#include "Utils/Png/lodepng.h"
#include "Core/Shaders/SkyBoxShader.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Transform.hpp"

void SkyBoxComponent::Construct()
{
	float Scale = 1000.0f;
	float CubePoints[] = {
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

	glGenVertexArrays(1, &CubeVertexArray);
	glBindVertexArray(CubeVertexArray);

	glGenBuffers(1, &CubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &CubePoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	/*
	CreateCubeMap("Resources/Textures/CubeMap/NegZ.png",
				  "Resources/Textures/CubeMap/PosZ.png",
				  "Resources/Textures/CubeMap/PosY.png",
				  "Resources/Textures/CubeMap/NegY.png",
				  "Resources/Textures/CubeMap/NegX.png",
				  "Resources/Textures/CubeMap/PosX.png"
	);
	*/
}

void SkyBoxComponent::Start()
{

}

void SkyBoxComponent::CreateCubeMap(const std::string Front, const std::string Back,
									const std::string Top, const std::string Bottom,
									const std::string Left, const std::string Right)
{
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &CubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeTexture);

	// load each image and copy into a side of the cube-map texture
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Back);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Front);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Top);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Bottom);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Left);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, Right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(0);
}

bool SkyBoxComponent::LoadCubeMapSide(GLenum SideTarget, const std::string TexturePath)
{
	std::vector<unsigned char> PngImage;
	unsigned Width, Height;
	unsigned PngLoadingError = lodepng::decode(PngImage, Width, Height, TexturePath);

	// If there's an error, display it.
	if (PngLoadingError != 0)
	{
		std::cout << "error " << PngLoadingError << ": " << lodepng_error_text(PngLoadingError) << std::endl;
		exit(1);
	}
	// non-power-of-2 dimensions check
	if ((Width & (Width - 1)) != 0 || (Height & (Height - 1)) != 0)
	{
		std::cout << "WARNING: image " << TexturePath << " is not power-of-2 dimensions" << std::endl;
	}
	glTexImage2D(SideTarget, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &PngImage[0]);
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
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeTexture);
		glBindVertexArray(CubeVertexArray);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(0);
		SkyBoxShader::GetInstance().Stop();
		glDepthMask(GL_TRUE);
	}
}

GLuint SkyBoxComponent::GetCubeMapTextureId() const
{
	return CubeTexture;
}
