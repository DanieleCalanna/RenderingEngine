#include "Core/Scene/SkyBox.hpp"
#include "Core/Camera/Camera.hpp"
#include <iostream>
#include "Utils/Png/lodepng.h"
#include "Core/Shaders/SkyBoxShader.hpp"
#include "Core/Shaders/StandardShader.hpp"
#include "Core/Transform.hpp"
#include "glm/ext.hpp"
#include "Core/Window.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

void SkyBoxComponent::Construct()
{
	float CubePoints[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &CubeVertexArray);
	glBindVertexArray(CubeVertexArray);

	glGenBuffers(1, &CubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &CubePoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	CubeTexture = GetCubemapFromHdr("Resources/Textures/HDR/Newport_Loft/Newport_Loft_8k.jpg", 4096);
	HdrTexture = GetCubemapFromHdr("Resources/Textures/HDR/Newport_Loft/Newport_Loft_Ref.hdr", 1024);
	
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

GLuint SkyBoxComponent::GetCubemapFromHdr(std::string HdrPath, GLsizei Size) const
{
	// Setup framebuffer
	unsigned int CaptureFBO;
	unsigned int CaptureRBO;
	glGenFramebuffers(1, &CaptureFBO);
	glGenRenderbuffers(1, &CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Size, Size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CaptureRBO);

	// Load the HDR environment map
	stbi_set_flip_vertically_on_load(true);
	int HdrWidth, HdrHeight, HdrComponents;
	float *HdrData = stbi_loadf(HdrPath.c_str(), &HdrWidth, &HdrHeight, &HdrComponents, 0);
	GLuint HdrTextureId;
	if (HdrData)
	{
		glGenTextures(1, &HdrTextureId);
		glBindTexture(GL_TEXTURE_2D, HdrTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, HdrWidth, HdrHeight, 0, GL_RGB, GL_FLOAT, HdrData); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(HdrData);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// Setup cubemap to render to and attach to framebuffer
	GLuint EnvCubemap;
	glGenTextures(1, &EnvCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, EnvCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Size, Size, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up projection and view matrices for capturing data onto the 6 cubemap face directions
	glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// Convert HDR environment map to cubemap equivalent
	Shader HdrToCubemapShader("Resources/Shaders/HdrToCubemapShader/HdrToCubemapVertex.glsl", "Resources/Shaders/HdrToCubemapShader/HdrToCubemapFragment.glsl");
	HdrToCubemapShader.Refresh();
	HdrToCubemapShader.Start();
	HdrToCubemapShader.LoadInt("HdrMap", 0);
	HdrToCubemapShader.LoadMat4("Projection", CaptureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, HdrTextureId);

	glViewport(0, 0, Size, Size); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		HdrToCubemapShader.LoadMat4("View", CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Render Cube
		glBindVertexArray(CubeVertexArray);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	HdrToCubemapShader.Stop();
	glViewport(0, 0, Window::GetSingletonWindow().GetWidth() , Window::GetSingletonWindow().GetHeight());
	return EnvCubemap;
}


void SkyBoxComponent::Update()
{
	const Camera* ActiveCamera = Camera::GetActiveCamera();
	if (ActiveCamera)
	{
		
		glDepthMask(GL_FALSE);
		SkyBoxShader::GetInstance().Start();
		SkyBoxShader::GetInstance().LoadInt("CubeMap", 0);
		SkyBoxShader::GetInstance().LoadProjectionMatrix(ActiveCamera->GetProjectionMatrix());
		SkyBoxShader::GetInstance().LoadViewMatrix(ActiveCamera->GetViewMatrix());
		SkyBoxShader::GetInstance().LoadTransformationMatrix(Transform(ActiveCamera->GetWorldTransform().Location, glm::vec3(0.0f), glm::vec3(1000.0f)).GetTransformationMatrix());
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
